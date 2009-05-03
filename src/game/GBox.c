/******************************************************************************\
 Dragoon - Copyright (C) 2008 - Michael Levin

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
\******************************************************************************/

#include "g_private.h"

/* Box class */
typedef struct GBoxClass {
        GEntityClass entity;
        PImpactType impact, impactOther;
        float mass, elasticity;
        bool debugPhysics;
} GBoxClass;

/* Box entity */
typedef struct GBox {
        PEntity entity;
        RSprite sprite;
} GBox;

/******************************************************************************\
 Box event function.
\******************************************************************************/
int GBox_eventFunc(GBox *box, int event, void *args)
{
        GBoxClass *boxClass = box->entity.entityClass;

        if (event == PE_UPDATE) {
                box->sprite.origin = box->entity.origin;
                box->sprite.size = box->entity.size;
                RSprite_draw(&box->sprite);
        } else if (CHECKED && boxClass->debugPhysics) {
                if (event == PE_PHYSICS) {
                        C_dump("y.txt", "%d\t%.2f", c_frame,
                               box->entity.origin.y);
                        C_dump("vel.txt", "%d\t%.2f", c_frame,
                               box->entity.velocity.y);
                } else if (event == PE_PHYSICS_DONE) {
                        C_dump("y.txt", "\t%.2f\n",
                               box->entity.origin.y);
                        C_dump("vel.txt", "\t%.2f\n",
                               box->entity.velocity.y);
                }
        }
        return 0;
}

/******************************************************************************\
 Spawn a fixture box.
\******************************************************************************/
GBox *GBox_spawn(GBoxClass *boxClass)
{
        GBox *box;

        C_new(&box);
        RSprite_init(&box->sprite, boxClass->entity.spriteName);
        box->entity.eventFunc = (PEventFunc)GBox_eventFunc;
        box->sprite.z = boxClass->entity.z;
        box->entity.mass = boxClass->mass;
        box->entity.elasticity = boxClass->elasticity;
        box->entity.impactOther = boxClass->impactOther;
        PEntity_spawn(&box->entity, "Box");
        PEntity_impact(&box->entity, boxClass->impact);
        return box;
}

/******************************************************************************\
 Parse a box class definition.
\******************************************************************************/
void GBox_parseClass(FILE *file, const char *className)
{
        GBoxClass *boxClass;
        const char *token;

        if (!(boxClass = CNamed_alloc(&g_classRoot, className,
                                      sizeof (GBoxClass), NULL, CNP_NULL))) {
                C_warning("Class '%s' already defined", className);
                return;
        }
        GEntityClass_init(&boxClass->entity);
        boxClass->entity.spawnFunc = (GSpawnFunc)GBox_spawn;

        /* Sprite defaults to class name */
        C_strncpy_buf(boxClass->entity.spriteName, className);
        boxClass->entity.size = R_spriteSize(className);

        /* Parse box class parameters */
        for (token = C_token(file); token[0]; token = C_token(file))
                if (GEntityClass_parseToken(&boxClass->entity, file, token));

                /* Impact class */
                else if (!strcasecmp(token, "impact"))
                        boxClass->impact = G_token_impact(file);

                /* Impact-other class */
                else if (!strcasecmp(token, "impactOther"))
                        boxClass->impactOther = G_token_impact(file);

                /* Mass */
                else if (!strcasecmp(token, "mass"))
                        boxClass->mass = C_token_float(file);

                /* Elasticity */
                else if (!strcasecmp(token, "elasticity"))
                        boxClass->elasticity = C_token_float(file);

                /* Debug physics */
                else if (CHECKED && !strcasecmp(token, "debugPhysics"))
                        boxClass->debugPhysics = TRUE;

                else
                        C_warning("Unknown box param '%s'", token);
}

