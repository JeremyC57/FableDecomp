#ifndef FABLE_ENGINE_CPHYSICSDEF_H
#define FABLE_ENGINE_CPHYSICSDEF_H
// Both original PDB records and native006afe60 loads and donor0246e600/0246e5c0 prove this layout.
#include "engine/CSubDefClassBase.h"
class CPhysicsDef : public CSubDefClassBase {
public:
    float Diameter;
    float Friction;
    float Elasticity;
    float AirResistance;
    float Mass;
    long InteractionFlags;
    bool PushableByHero;
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsDef)==68);
#endif
