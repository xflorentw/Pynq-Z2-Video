###############################################################################
 # @file create_proj.tcl
 #  Author: Florent Werbrouck
 #	Tcl script for re-creating Video project
 # 
 #
###############################################################################
 # Copyright (c) 2021, Florent Werbrouck
 # All rights reserved.
 # 
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions are met:
 # 
 # 1. Redistributions of source code must retain the above copyright notice, this
 #    list of conditions and the following disclaimer.
 # 
 # 2. Redistributions in binary form must reproduce the above copyright notice,
 #    this list of conditions and the following disclaimer in the documentation
 #    and/or other materials provided with the distribution.
 # 
 # 3. Neither the name of the copyright holder nor the names of its
 #    contributors may be used to endorse or promote products derived from
 #    this software without specific prior written permission.
 # 
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 # AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 # DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 # FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 # DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 # SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 # CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 # OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 #
###############################################################################

# Check if script is running in correct Vivado version.
set scripts_vivado_version "2023.2"
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts "The version $current_vivado_version is not supported. Supported versions are $scripts_vivado_version"
   return 1
}
# Configuration - Can be modified by the user
set project_name 				hw_proj
set BD_name 					video_out_pynq_z2

# Create a new project
create_project $project_name ./$project_name -part xc7z020clg400-1
set_property target_language Verilog [current_project]

# Create 'sources_1' fileset (if not found)
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1
}
# Add hdl files
#add_files -fileset sources_1 -norecurse -scan_for_includes ./src/hdl
#import_files -fileset sources_1 -norecurse ./src/hdl

# Create 'constrs_1' fileset (if not found)
if {[string equal [get_filesets -quiet constrs_1] ""]} {
  create_fileset -constrset -quiet constrs_1
}
# Add constraint files
add_files -fileset constrs_1 -norecurse -scan_for_includes ./src/constr
import_files -fileset constrs_1 -norecurse ./src/constr

# Create 'sim_1' fileset (if not found)
if {[string equal [get_filesets -quiet sim_1] ""]} {
  create_fileset -simset sim_1
}
# Add simulation files
#add_files -fileset sim_1 -norecurse -scan_for_includes ./src/sim
#import_files -fileset sim_1 -norecurse ./src/sim


# Add ip repository
set_property  ip_repo_paths  ../vivado-library [current_project]
update_ip_catalog

set design_name 	$BD_name

# Build the Block Design
source ./src/bd.tcl

# Validate the BD
regenerate_bd_layout
#validate_bd_design 
save_bd_design

#Generate the wrapper
make_wrapper -files [get_files ${BD_name}.bd] -top

# Add the wrapper to the fileset
set obj [get_filesets sources_1]
set files [list "[file normalize [glob "./$project_name/$project_name.gen/sources_1/bd/$BD_name/hdl/${BD_name}_wrapper.v"]]"]
add_files -norecurse -fileset $obj $files

# Generate the output products
generate_target all [get_files ./$project_name/$project_name.srcs/sources_1/bd/$BD_name/${BD_name}.bd]
create_ip_run [get_files -of_objects [get_fileset sources_1] ./$project_name/$project_name.srcs/sources_1/bd/$BD_name/${BD_name}.bd]
launch_runs -jobs 8 [get_runs $BD_name*synth_1]

# Run Synthesis
launch_runs synth_1 -jobs 8
wait_on_run synth_1

# Run Implementation and generate the bitstream
launch_runs impl_1 -jobs 8
wait_on_run impl_1
launch_runs impl_1 -to_step write_bitstream -jobs 8
wait_on_run impl_1

# Export xsa
write_hw_platform -fixed -include_bit -force -file ./${BD_name}.xsa

exit
