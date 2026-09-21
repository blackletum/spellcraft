import sys

from mesh_export.entities import camera_animation

def process_scene():
    output_filename = sys.argv[-1]

    with open(output_filename, 'wb') as file:
        file.write('CANM'.encode())

        camera_animation.export_camera_animations(output_filename.replace('.canim', '.sanim'), file)

process_scene()