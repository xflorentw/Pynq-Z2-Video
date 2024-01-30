import os
import vitis

client = vitis.create_client()
client.set_workspace(path='vitis_workspace')
app = client.create_app_component(name='vid_out_app', platform=os.getcwd() + '/vitis_workspace/platform/export/platform/platform.xpfm', domain='standalone_ps7_cortexa9_0', template='empty_application')
app.import_files(from_loc='src', files=os.listdir('src'), dest_dir_in_cmp='src')
app.build()
