// NThingShot::CTCShotDefinitionFireballSpell::GetStatToModifyOnHit(EHeroExperienceStatCategory&) -- a fireball hit trains Will
// Leaf: ?GetStatToModifyOnHit@CTCShotDefinitionFireballSpell@NThingShot@@UAE_NAAW4EHeroExperienceStatCategory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCB11, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 30 of NThingShot::CTCShotDefinitionFireballSpell's vtable 0x01241064 (the slot holds this body in 1 vtable(s), owner NThingShot::CTCShotDefinitionFireballSpell); the Xbox 360 Test PDB virtual order (third source) aligned on 25 TLC-named slots of that vtable puts GetStatToModifyOnHit(EHeroExperienceStatCategory &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetStatToModifyOnHit@CTCShotDefinitionFireballSpell@NThingShot@@UAE_NAAW4EHeroExperienceStatCategory@@@Z; `ret` matches the signature.
// Was: NThingShot::CTCShotDefinitionFireballSpell::vfunc_30 (rebuild/src/compiled/00/4d/unattributed_vfunc30_004dcb11.cpp).
// No shared header for NThingShot::CTCShotDefinitionFireballSpell: minimal local definition on its shared base
// NThingShot::CTCShotDefinitionBase (RTTI: via CTCShotDefinitionProjectile, which has no header and adds nothing this body uses).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThing.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
namespace NThingShot {
class CTCShotDefinitionFireballSpell : public CTCShotDefinitionBase {
public:
    virtual bool GetStatToModifyOnHit(EHeroExperienceStatCategory&); // slot 30
};
bool CTCShotDefinitionFireballSpell::GetStatToModifyOnHit(EHeroExperienceStatCategory& category)
{
    category = HERO_STAT_WILL;
    return true;
}
}
