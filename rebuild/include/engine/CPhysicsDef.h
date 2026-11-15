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
#ifdef FABLE_RETAIL_CPHYSICSDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CPhysicsDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CPhysicsDef)==68);
#endif
