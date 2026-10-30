// CQ_BreakSiegeScript::WatchForTermination() -- put Knothole Glade in village limbo with the siege theme and no decals (again each time the region reloads) until the quest ends; then lift the limbo, remove the blockage, open the gate, play the end cutscene, fail (with the failure reason text) or complete the quest, restore theme and decals, fade in, and once Knothole Glade unloads deactivate the quest
// Leaf: ?WatchForTermination@CQ_BreakSiegeScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1D7D0, length 1253 bytes.
// Callees: CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, Compare 0x00411570
//   (the inlined CCharString == const char*), CWideString() 0x0099AED0 / operator=(const wchar_t*) 0x0099B800 / ~CWideString
//   0x0099B510, ~CScriptThing 0x004AA840, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CDefStringTable::GetString
//   0x009D49B0 on g_DefStringTable 0x013CA828 (index pGlobalScriptDef 0x0143E90C +0xbb0); CScriptThing::IsAlive (slot 75);
//   the game script interface (+0x40) slots 7, 12, 22, 72, 108, 133, 281, 288, 289, 374, 376, 425, 440, 655, 656, 657, 688.
// Strings: "VILLAGE_KNOTHOLEGLADE" 0x012CAECC, "KnotholeGlade" 0x012C513C, "Blockage" 0x012CAEC0, "KG_Gate" 0x012C31A8,
//   L"TEXT_QST_018_QUEST_FAILED_PRISONER_DIED" 0x012CAE70, L"..._CHIEF_DIED" 0x012CAE20, L"..._PRISONER_ESCAPED" 0x012CADC8,
//   L"There is no reason" 0x012CAD9C.
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, EndCutsceneStarts +0x53,
//   EndCutsceneDone +0x54, ReasonForFailure +0x58; the master data's +0xe0 flag.
// Shape: region-loaded loop and inline CCharString == of the landed CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor
//   0x00D151D0; generated script code (`if (IsActiveThreadTerminating()) return;` after every statement).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CWideString { public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
    const CWideString& operator=(const wchar_t* text);      // 0x0099B800
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
struct CDefStringTable {
    CCharString GetString(long index);                      // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828
class CScriptDef { public:
    unsigned char _pad[0xbb0];
    long KnotholeGladeSiegeTheme;                           // +0xbb0 (string-table index of the siege music theme)
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_BreakSiegeMasterData {
    unsigned char _pad[0xe0];
    bool BreakSiegeCompleted;                               // +0xe0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_BreakSiegeScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                          // +0x40
    CQ_BreakSiegeMasterData* Data;                          // +0x44
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool BanditDemandsKnown;                                // +0x4a
    bool ChiefRefusesDemands;                               // +0x4b
    bool BanditLeaderFree;                                  // +0x4c
    bool BanditSiegeStart;                                  // +0x4d
    bool HasFirstWaveAttacked;                              // +0x4e
    bool StartSecondWave;                                   // +0x4f
    bool BanditsAttackHero;                                 // +0x50
    bool HeroLetInTown;                                     // +0x51
    bool GuardFollowingHero;                                // +0x52
    bool EndCutsceneStarts;                                 // +0x53
    bool EndCutsceneDone;                                   // +0x54
    bool AttackedBanditLeader;                              // +0x55
    bool QuestStartScreened;                                // +0x56
    long ReasonForFailure;                                  // +0x58
    void WatchForTermination();
};
void CQ_BreakSiegeScript::WatchForTermination()
{
    CScriptThing village = Game->GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE");
    CCharString region;
    Game->SetVillageLimbo(village, true);
    {
        CCharString theme = g_DefStringTable.GetString(pGlobalScriptDef->KnotholeGladeSiegeTheme);
        Game->TransitionToTheme(theme, 0.5f);
    }
    Game->EnableDecals(false);
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->MsgOnRegionLoaded(region) && region == "KnotholeGlade") {
            if (IsActiveThreadTerminating())
                return;
            Game->SetVillageLimbo(village, true);
            {
                CCharString theme = g_DefStringTable.GetString(pGlobalScriptDef->KnotholeGladeSiegeTheme);
                Game->TransitionToTheme(theme, 0.5f);
            }
            Game->EnableDecals(false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetVillageLimbo(village, false);
    if (Game->GetThingWithScriptName("Blockage").IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Game->GetThingWithScriptName("Blockage"), false, true);
    }
    CScriptThing gate = Game->GetThingWithScriptName("KG_Gate");
    Game->OpenDoor(gate);
    EndCutsceneStarts = true;
    while (!EndCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetWeaponOutCrimeEnabled(true);
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        CWideString reason;
        if (ReasonForFailure == 1) {
            if (IsActiveThreadTerminating())
                return;
            reason = L"TEXT_QST_018_QUEST_FAILED_PRISONER_DIED";
        } else if (ReasonForFailure == 2) {
            if (IsActiveThreadTerminating())
                return;
            reason = L"TEXT_QST_018_QUEST_FAILED_CHIEF_DIED";
        } else if (ReasonForFailure == 3) {
            if (IsActiveThreadTerminating())
                return;
            reason = L"TEXT_QST_018_QUEST_FAILED_PRISONER_ESCAPED";
        } else {
            if (IsActiveThreadTerminating())
                return;
            reason = L"There is no reason";
        }
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, reason, true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Data->BreakSiegeCompleted = true;
    }
    Game->ResetToDefaultTheme(0.2f);
    Game->EnableDecals(true);
    Game->Pause(0.1f);
    Game->FadeScreenIn();
    while (Game->IsRegionLoaded("KnotholeGlade") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}