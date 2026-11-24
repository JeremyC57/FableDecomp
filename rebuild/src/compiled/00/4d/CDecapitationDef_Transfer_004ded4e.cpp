// Leaf: ?Transfer@CDecapitationDef@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_CDECAPITATIONDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#pragma optimize("s",on)
#include "engine/CDecapitationDef.h"

struct CMember { int x; };
class CPersistContext { public: template <class T> void Transfer(T& value); };

void CDecapitationDef::Transfer(class CPersistContext& ctx)
{
    CDecapitationDef* def = this;
    ctx.Transfer(*(long*)((CMember*)&def->BodyParticleEffect));
    ctx.Transfer(*(long*)((CMember*)&def->Head));
}
// Flatten route (flatten_pilot v5, NF330): CDecapitationDef_Transfer -> CDecapitationDef::Transfer, the RTTI-placed PDB member; class model: engine/CDecapitationDef.h opt-in FABLE_RETAIL_CDECAPITATIONDEF_VIRTUAL_MODEL, FABLE_RETAIL_DEF_CLASS_BASE.
