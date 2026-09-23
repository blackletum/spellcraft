import sys

from mesh_export.parse import struct_parse  

with open(sys.argv[-2], 'r') as f:
    file_content = f.read()

structures = struct_parse.find_structs(file_content)

def_counts: dict[str, int] = {}

with open(sys.argv[-1], 'w') as f:
    f.write('#include "fields.h"\n')
    f.write('#include <stddef.h>\n\n')


    for name, struct in structures.items():
        if not name.endswith('_definition'):
            continue

        def_name = name[0:-len('_definition')]

        f.write(f"struct entity_field_type_location fields_{def_name}[] = {{")

        def_count = 0

        for field in struct.children:
            field_type = None

            if struct_parse.is_string_type(field.data_type):
                field_type = 'ENTITY_FIELD_TYPE_STRING'
            elif field.data_type == 'line_mesh_data_ref':
                field_type = 'ENTITY_FIELD_TYPE_LINE_MESH'

            if field_type:
                f.write(f"\n    {{ .offset = offsetof(struct {name}, {field.name}), .type = {field_type} }},")
                def_count = def_count + 1

        if def_count > 0:
            f.write("\n};\n")
        else:
            f.write("};\n")

        def_counts[def_name] = def_count
            
with open(sys.argv[-1][:-2] + '.h', 'w') as f:
    f.write('#include "entity_spawner.h"\n\n')

    for name, struct in structures.items():
        if not name.endswith('_definition'):
            continue

        def_name = name[0:-len('_definition')]

        f.write(f"extern struct entity_field_type_location fields_{def_name}[];\n")
        f.write(f"#define FIELD_COUNT_{def_name} {def_counts[def_name]}\n\n")