import os
import vitis

client = vitis.create_client()
client.set_workspace(path='vitis_workspace')
platform = client.create_platform_component(name='platform', hw='../hw/video_out_pynq_z2.xsa', os='standalone', cpu='ps7_cortexa9_0')
status = platform.build()

