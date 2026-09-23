#include "rune_upgrade.h"    
    
#include "../collision/shapes/cylinder.h"

static dynamic_object_type_t rune_upgrade_collider = {
    CYLINDER_COLLIDER(0.9f, 1.0f),
    .center = {0.0f, 1.0f, 0.0f},  
};

void rune_upgrade_interact(interactable_t* interactable, entity_id from) {

}

void rune_upgrade_init(rune_upgrade_t* rune_upgrade, struct rune_upgrade_definition* definition, entity_id entity_id) {
    transformSaInit(&rune_upgrade->transform, &definition->position, &definition->rotation, 1.0f);

    renderable_single_axis_init(&rune_upgrade->renderable, &rune_upgrade->transform, definition->mesh);
    render_scene_add_renderable(&rune_upgrade->renderable, 0.0f);

    dynamic_object_init(
        entity_id,
        &rune_upgrade->collider,
        &rune_upgrade_collider,
        COLLISION_LAYER_Z_TARGET | COLLISION_LAYER_TANGIBLE,
        &rune_upgrade->transform.position,
        &rune_upgrade->transform.rotation
    );

    rune_upgrade->collider.is_fixed = true;
    rune_upgrade->collider.weight_class = WEIGHT_CLASS_SUPER_HEAVY;

    collision_scene_add(&rune_upgrade->collider);

    interactable_init(&rune_upgrade->interactable, entity_id, INTERACT_TYPE_CHECK, rune_upgrade_interact, rune_upgrade);
}

void rune_upgrade_destroy(rune_upgrade_t* rune_upgrade, struct rune_upgrade_definition* definition) {
    renderable_destroy(&rune_upgrade->renderable);
    render_scene_remove(&rune_upgrade->renderable);

    collision_scene_remove(&rune_upgrade->collider);

    interactable_destroy(&rune_upgrade->interactable);
}

void rune_upgrade_common_init() {

}

void rune_upgrade_common_destroy() {

}
