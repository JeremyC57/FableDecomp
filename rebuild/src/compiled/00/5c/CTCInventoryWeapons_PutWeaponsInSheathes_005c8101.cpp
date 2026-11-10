// CTCInventoryWeapons::PutWeaponsInSheathes() -- takes the weapons out of the hands, then puts each live active weapon (melee, then ranged) into its passive carry slot on the owner, the two sharing one running stack offset
// Leaf: ?PutWeaponsInSheathes@CTCInventoryWeapons@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C8101, 117 bytes, thiscall plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?PutWeaponsInSheathes@CTCInventoryWeapons@@QAEXXZ (Ego_r donor 0x006280DB, between
//   ConstructWieldEntryPC and FinalConstructWeaponItemEntriesPC); callers RefreshSheathedWeapons 0x005C9962 and the carrier code.
// Callees: RemoveWeaponsFromSheathes 0x005C552C (landed; Ego_r spelling), std::bitset<274>::test 0x00410DE0 (the carrying
//   component's presence test), IS_THING_ALIVE 0x004CC340, CBaseIntelligentPointer::GetPItem 0x00A01B50, and the TU's file-static
//   PutSheatheObjectInCarrySlot 0x005C36C2 (Ego_r ?PutSheatheObjectInCarrySlot@?A0x0a34196c@@YIXAAVCThing@@0AAM@Z, `__fastcall`).
// Shape: the owner's carrying component is fetched and not used (an assertion-only lookup): GetTC<CTCCarrying> with the CVectorMap
//   Find expanded in place (FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND, as the landed 0x005C552C / 0x005C36C2 of this TU), so the pure
//   Find is dropped and only the out-of-line presence test survives, through the `mov eax,[esi+4] ; lea ecx,[eax+20h]` temporary.
//   The owner is named before each call, which loads it ahead of the offset push (ebx shrink-wrapped to the melee block).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

namespace {
void FABLE_FASTCALL PutSheatheObjectInCarrySlot(CThing& owner, CThing& weapon, float& offset);   // 0x005C36C2 (file static)
}

void CTCInventoryWeapons::PutWeaponsInSheathes()
{
    RemoveWeaponsFromSheathes();
    CTCCarrying* carrying;                      // looked up for an assertion only: the retail build keeps just the presence test
    ParentThing->GetTC(&carrying);
    float offset = 0.0f;                        // the stack offset both sheathes share
    if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
        CThing& owner = *ParentThing;
        PutSheatheObjectInCarrySlot(owner, *PActiveMeleeWeapon.Get(), offset);
    }
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CThing& owner = *ParentThing;
        PutSheatheObjectInCarrySlot(owner, *PActiveRangedWeapon.Get(), offset);
    }
}