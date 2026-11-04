// CTCHero::SetWeaponsAsUsable(bool usable) -- record whether the hero may use weapons; when they become unusable and the hero is carrying a weapon, take it out of the carrying component's primary slot
// Leaf: ?SetWeaponsAsUsable@CTCHero@@QAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CA380, 115 bytes, `ret 4` (both RemoveThingFromPrimarySlot arms tail-jump with the argument
//   slot rewritten to false); Ego_r.pdb ?SetWeaponsAsUsable@CTCHero@@QAEX_N@Z.
// Callees: static __fastcall CTCCarrying::IsThingCarryingWeapon(const CThing&) 0x006A1540, LowerBound 0x0040F020 (CThing::GetTC<CTCCarrying>
//   through Components.Find), CTCCarrying::RemoveThingFromPrimarySlot(bool) 0x006A1E00.
// Layout: Ego_r.pdb CTCHero WeaponsUsable +0x0C over the retail CTCBase; no engine header exists for CTCHero.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCCARRYING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"

class CTCHero : public CTCBase {
public:
    void SetWeaponsAsUsable(bool usable);
private:
    bool WeaponsUsable;                                             // +0x0c
};

void CTCHero::SetWeaponsAsUsable(bool usable)
{
    WeaponsUsable = usable;
    if (!usable && CTCCarrying::IsThingCarryingWeapon(GetParentThing())) {
        CTCCarrying* carrying;
        GetParentThing().GetTC(&carrying);
        carrying->RemoveThingFromPrimarySlot(false);
    }
}