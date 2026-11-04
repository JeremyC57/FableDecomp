// NThingShot::CTCShotDefinitionBase::GetPWeapon() const -- the weapon that fired this shot (PWeapon's thing, or 0)
// Leaf: ?GetPWeapon@CTCShotDefinitionBase@NThingShot@@QBEPAVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BEFF0, 8 bytes: `add ecx,30h ; jmp 0x00A01B50` (CBaseIntelligentPointer::GetPItem on PWeapon).
// Identity (two sources): Ego_r.pdb ?GetPWeapon@CTCShotDefinitionBase@NThingShot@@QBEPAVCThing@@XZ 0x007BDEE0, the body after
//   CanBeHitByMeleeStrike 0x007BDE80 / GetInitialVelocity 0x007BDE90, as retail 0x007BEFF0 follows the landed 0x007BEFA0 / 0x007BEFB0
//   in the shot-definition TU; Ego_r's PWeapon is CIntelligentPointer<CThing> at PDB +0x34 = retail +0x30 (one STLport vector before
//   it). The landed caller 0x00811760 binds the address as ?GetPWeapon@CTCWeaponHolder_77@@QAEPAVCThing@@XZ (placeholder class,
//   NAME_FIXES.tsv); the catalog's garbled `_Cons_val<...CParticleMorphs...>` label is propagated.
// Shape: the inline CIntelligentPointer<CThing>::Get() (static_cast of GetPItem) on the member, tail-jumped.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Header: engine/NThingShot_CTCShotDefinitionBase.h (new).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
namespace NThingShot {
CThing* CTCShotDefinitionBase::GetPWeapon() const
{
    return PWeapon.Get();
}
}