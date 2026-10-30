// NScript::CQS_GuardianSisterInfoScript::Main() -- the Guardian-sister info quest main: bind the MazeAtTavern entity script, post-add the scripted entities, set the quest card objective (TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01, BowerstoneSlums), wait until BowerstoneSlums is loaded, then set the time of day to 10
// Leaf: ?Main@CQS_GuardianSisterInfoScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25AB0, 423 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, from slot 2 (Main) of the RTTI-less quest vtable 0x012E1DAC (0x012E1DB4) that
//   the landed NScript::CQS_GuardianSisterInfoScript ctor 0x00E259D0 stores (slot 1 the landed RegisterMain 0x00E25A10, which
//   spawns slot 2 as "Main"); Ego_r.pdb ?Main@CQS_GuardianSisterInfoScript@NScript@@UAEXXZ (donor 0x00E70EE0, after
//   RegisterMain 0x00E70DB0 as here). The binding factory 0x00E25C70 news 0x28 bytes and stores the entity vtable 0x012E1E28
//   (Main 0x00E25F70, Init 0x00E25D20, OnPredicateFail 0x00E25D30): Ego_r CMazeAtTavern is 40 bytes with exactly Init,
//   OnPredicateFail, Alloc and Main, and the binding name is "MazeAtTavern". Catalog label sub_00e25ab0.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940,
//   CMazeAtTavern::Alloc 0x00E25C70 (Ego_r spelling; catalog label CTriangleList); vtable CEntityScriptBinding<T> 0x012E1E10;
//   CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 296 SetQuestCardObjective,
//   646 SetTimeOfDay, 655 GetActiveQuestName (ghidra_out/cgsi_slots_aligned.tsv).
// Literals: "MazeAtTavern" 0x012E1E18, "BowerstoneSlums" 0x012CB55C (three times), "" 0x0122D70E,
//   "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01" 0x012E1DDC; 10.0f is the immediate 0x41200000.
// Layout: CScriptBase retail 0x40 bytes (polymorphic), Game +0x40; CEntityScriptBinding 0x1c bytes (Flags 0).
// Model: the landed NScript::CQS_GuardianTrophyDealerInfoScript::Main 0x00E27920 (binding new-expression with its conditional
//   name temporary, SetQuestCardObjective strings built right to left); the wait is the rotated
//   `while (!Game->IsRegionLoaded(..)) { NewScriptFrame(); if (term) return; }` (`!call()` gives neg/sbb/inc).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CEntityScriptBinding.h"                // CEntityScriptBinding<T> (0x1c, vtable 0x012E1E10 here)
#include "engine/CGameScriptInterface.h"               // CGameScriptInterfaceBase (917 slots) + vtable 0x01260F0C
#include "engine/CScriptBase.h"                        // retail 0x40, vtable 0x012C1648
namespace NScript {
class CQS_GuardianSisterInfoScript : public CScriptBase {
public:
    class CMazeAtTavern {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQS_GuardianSisterInfoScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E25C70
    };
    CGameScriptInterface* Game;                         // +0x40
    virtual void RegisterMain();                        // slot 1 0x00E25A10
    virtual void Main();                                // slot 2 0x00E25AB0 (this row)
};

void CQS_GuardianSisterInfoScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_GuardianSisterInfoScript>("MazeAtTavern", this, &CMazeAtTavern::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "", "BowerstoneSlums");
    while (!Game->IsRegionLoaded("BowerstoneSlums")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetTimeOfDay(10.0f);
}
}   // namespace NScript