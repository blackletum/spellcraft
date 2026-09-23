#ifndef __ENTITIES_RUNE_UPGRADE_H__
#define __ENTITIES_RUNE_UPGRADE_H__

#include "../math/vector3.h"
#include "../entity/entity_id.h"
#include "../scene/scene_definition.h"
#include "entity_deps.h"

struct rune_upgrade {
    transform_sa_t transform;
    renderable_t renderable;
    animator_t animator;

    dynamic_object_t collider;
    interactable_t interactable;
};

typedef struct rune_upgrade rune_upgrade_t;

void rune_upgrade_init(rune_upgrade_t* rune_upgrade, struct rune_upgrade_definition* definition, entity_id entity_id);
void rune_upgrade_destroy(rune_upgrade_t* rune_upgrade, struct rune_upgrade_definition* definition);
void rune_upgrade_common_init();
void rune_upgrade_common_destroy();

#endif