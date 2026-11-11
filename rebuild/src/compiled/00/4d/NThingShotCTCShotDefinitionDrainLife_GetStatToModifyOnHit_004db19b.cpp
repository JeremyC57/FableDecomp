// NThingShot::CTCShotDefinitionDrainLife::GetStatToModifyOnHit(EHeroExperienceStatCategory&) -- returns false
// Leaf: ?GetStatToModifyOnHit@CTCShotDefinitionDrainLife@NThingShot@@UAE_NAAW4EHeroExperienceStatCategory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB19B, thiscall, `ret 4`.
// Identity: slot 30 of NThingShot::CTCShotDefinitionDrainLife's vtable 0x012403F4 (defined here: the slot holds this body in 1 vtable(s), all of NThingShot::CTCShotDefinitionDrainLife or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 25 TLC-named slots of that vtable puts GetStatToModifyOnHit(EHeroExperienceStatCategory &) at this slot; Ego_r/FableWin name set: ?GetStatToModifyOnHit@CTCShotDefinitionDrainLife@NThingShot@@UAE_NAAW4EHeroExperienceStatCategory@@@Z; `ret` matches the signature.
#include "engine/EHeroExperienceStatCategory.h"
namespace NThingShot {
class CTCShotDefinitionDrainLife {
public:
    virtual bool GetStatToModifyOnHit(EHeroExperienceStatCategory&);
};
bool CTCShotDefinitionDrainLife::GetStatToModifyOnHit(EHeroExperienceStatCategory&)
{
    return false;
}
}
