#include "camera_animation.h"

#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <libdragon.h>
#include "../util/hash_map.h"
#include "../util/file.h"

void camera_animation_list_init(struct camera_animation_list* list, int count, uint32_t rom_location) {
    list->animations = malloc(sizeof(struct camera_animation) * count);
    list->animation_count = count;
    list->rom_location = rom_location;

    for (int i = 0; i < count; ++i) {
        list->animations[i].name = NULL;
    }
}

void camera_animation_list_destroy(struct camera_animation_list* list) {
    for (int i = 0; i < list->animation_count; ++i) {
        free(list->animations[i].name);
    }
    free(list->animations);
}

struct camera_animation* camera_animation_lookup(struct camera_animation_list* list, const char* name) {
    struct camera_animation* end = list->animations + list->animation_count;

    for (struct camera_animation* curr = list->animations; curr < end; ++curr) {
        if (strcmp(name, curr->name) == 0) {
            return curr;
        }
    }

    return NULL;
}

void camera_animations_load(struct camera_animation_list* list, const char* filename, FILE* file) {
    uint16_t count;
    fread(&count, sizeof(count), 1, file);
    camera_animation_list_init(list, count, 0);

    int filename_len = strlen(filename);
    char modified_filename[filename_len + 1];
    strcpy(modified_filename, filename + strlen("rom:/"));
    strcpy(filename_find_ext(modified_filename), ".sanim");

    list->rom_location = dfs_rom_addr(modified_filename);

    for (int i = 0; i < count; ++i) {
        uint8_t name_length;
        fread(&name_length, 1, 1, file);
        char* name = malloc(name_length + 1);
        fread(name, name_length, 1, file);
        name[name_length] = '\0';

        struct camera_animation* animation = &list->animations[i];

        animation->name = name;
        fread(&animation->frame_count, sizeof(uint16_t), 1, file);
        fread(&animation->rom_offset, sizeof(uint32_t), 1, file);
        animation->rom_offset += list->rom_location;
    }
}

static hash_map_t animation_map_lookup;

void camera_animation_map(camera_animation_list_t* list, entity_id id) {
    if (!hash_map_is_ready(&animation_map_lookup)) {
        hash_map_init(&animation_map_lookup, 4);
    }

    hash_map_set(&animation_map_lookup, id, list);
}

void camera_animation_unmap(entity_id id) {
    hash_map_delete(&animation_map_lookup, id);

    if (animation_map_lookup.count == 0) {
        hash_map_destroy(&animation_map_lookup);
    }
}

camera_animation_list_t* camera_animation_get(entity_id id) {
    return hash_map_get(&animation_map_lookup, id);
}