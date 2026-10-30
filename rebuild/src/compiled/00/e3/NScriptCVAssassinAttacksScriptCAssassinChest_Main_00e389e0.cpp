// NScript::CV_AssassinAttacksScript::CAssassinChest::Main() -- the assassin's treasure chest: wait until the hero is within AssassinChestProximity, play SND_CHESTOPEN and CHEST_OPEN, spawn a CREATURE_ASSASSIN ("ChestHidingAssassin") at the chest, stock it with OBJECT_TREASURE_CLUE_3 when the hero has no clue yet (else OBJECT_FISH_CARP_MEDIUM_01) and face it the chest's way
// Leaf: ?Main@CAssassinChest@CV_AssassinAttacksScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E389E0, 488 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): the row is referenced once, from slot 1 (Main) of the RTTI-less entity vtable 0x012E3738 (0x012E373C),
//   which the landed NScript::CV_AssassinAttacksScript::CAssassinChest::Alloc 0x00E388F0 stores (site 0x00E38926); Ego_r.pdb
//   gives CAssassinChest exactly Alloc, Init, GetParentScript, OnPredicateFail and ?Main@CAssassinChest@CV_AssassinAttacksScript@
//   NScript@@EAEXXZ (private virtual). The vtable sits right after the "SCRIPT_JUMP_FROM_CHEST" literal. Catalog `sub_00e389e0`.
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30,
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840; CScriptThing slots 6 GetPos,
//   10 GetAngleXY; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature, 184 IsObjectInThingsPossession,
//   474 EntitySetFacingAngle, 487 EntityPlayObjectAnimation, 585 AddItemToContainer, 690 PlaySoundOnThing
//   (ghidra_out/cgsi_slots_aligned.tsv; signatures from the mangled names there).
// Data: pGlobalScriptDef 0x0143E90C (CScriptDef::AssassinChestProximity at retail +0x3A4 = Ego_r 0x3C8 - 0x24: nine Dinkumware
//   vectors before it are 4 bytes larger than STLport's); literals SND_CHESTOPEN 0x012E3770, CHEST_OPEN 0x01269568,
//   ChestHidingAssassin 0x012E3674, CREATURE_ASSASSIN 0x012E36D4, OBJECT_TREASURE_CLUE_3 0x012E35E0,
//   OBJECT_FISH_CARP_MEDIUM_01 0x012E3754 (each paired with retail by its relocation, rel.py).
// Layout: Ego_r.pdb CAssassinChest (28 bytes = the landed Alloc's new size): CEntityScriptBase {vptr, Game +4, Me +8},
//   ParentClass +0x14, PMasterData +0x18.
// Shape: the rotated distance wait of the landed CCrate1::Main 0x00EF54A0, the CreateCreature result kept as a named local (its
//   out-of-line dtor runs at the single later exit), the possession test kept as a `!` bool (`neg ; sbb ; inc`) and the
//   termination test repeated inside both arms of the clue choice: VC7.1 hoists the two identical IsActiveThreadTerminating calls
//   above the `test bl,bl` branch and keeps one `test al,al` per arm, as retail does (one test before the if/else gave the
//   term test first, 482v488).
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
class CScriptDef { public:
    unsigned char _pad[0x3a4];
    long AssassinChestProximity;                        // +0x3a4 (Ego_r 0x3c8 - 0x24)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AssassinAttacksScript {
public:
    class CAssassinChest;
};
class CV_AssassinAttacksScript::CAssassinChest : public CEntityScriptBase {
public:
    CV_AssassinAttacksScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;              // +0x18
private:
    virtual void Main();
};

void CV_AssassinAttacksScript::CAssassinChest::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->AssassinChestProximity)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->PlaySoundOnThing(Me, "SND_CHESTOPEN");
    Game->EntityPlayObjectAnimation(Me, "CHEST_OPEN", false);
    CScriptThing assassin = Game->CreateCreature("CREATURE_ASSASSIN", Me.GetPos(), "ChestHidingAssassin", false);
    bool noClue = !Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3", Game->GetHero());
    if (noClue) {
        if (IsActiveThreadTerminating())
            return;
        Game->AddItemToContainer(assassin, "OBJECT_TREASURE_CLUE_3");
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->AddItemToContainer(assassin, "OBJECT_FISH_CARP_MEDIUM_01");
    }
    Game->EntitySetFacingAngle(assassin, Me.GetAngleXY(), true);
}
}   // namespace NScript