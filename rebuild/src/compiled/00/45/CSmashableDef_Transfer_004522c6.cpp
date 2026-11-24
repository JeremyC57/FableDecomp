// Leaf: ?Transfer@CSmashableDef@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_CSMASHABLEDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#pragma optimize("s",on)
#include "engine/CSmashableDef.h"  // retyped onto the PDB layout; byte parity re-verified

class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void PersistB(unsigned char* p);
};

void CSmashableDef::Transfer(class CPersistContext& ctx)
{
    CSmashableDef* def = this;
    ctx.Transfer(*(bool*)(reinterpret_cast<unsigned char*>(&def->Smashable)));
    ctx.PersistB(reinterpret_cast<unsigned char*>(&def->ReplacementObject_Val));
    ctx.Transfer(*(long*)(reinterpret_cast<int*>(&def->SmashParticleEmitter)));
}
// Flatten route (flatten_pilot v5, NF330): CSmashableDef_Transfer -> CSmashableDef::Transfer, the RTTI-placed PDB member; class model: engine/CSmashableDef.h opt-in FABLE_RETAIL_CSMASHABLEDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
