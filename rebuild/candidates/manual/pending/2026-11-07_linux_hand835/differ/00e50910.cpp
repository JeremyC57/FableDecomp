// NScript::CV_BeardyBaldyScript::WatchForQuestCardConditions() -- wait for Phase 1, give the Beardy Baldy quest card (objective 02) and send the hero to the barber; then forever: on any new hair item set objective 03 and mark Beardy Baldy, wait for BowerstoneSlumsWarehouses to load, and when he lives and has been talked to by the hero restore objective 02 (unless Phase 3 is done), then back to the barber
// Grade: DIFFER(1392v1328) -- 57 masked lines: VC7.1 keeps the ebx zero live into the MsgIsTalkedToBy wait (`cmp ecx,ebx` at the loop head and an `xor ebx,ebx; jmp` back-edge) where retail uses `test ecx,ecx` and a plain `je` back-edge that falls into the shared epilogue placed right after the loop; so the epilogue and both inline-dtor tails sit at the end in ours
// Next idea: chunk 835 e1 (`PImp.Data && ...` MsgIsTalkedToBy body: 1395, 89), e2 (for(;;) with the Phase3 block inside the talked branch + break: 57, unchanged), e3 (do { if (talked) break; NSF } while (!T): 1375, 71); 22 grades in all. Next: a shape where the ebx zero is not live-in at the talk-loop head (e.g. the flag kept in a non-ebx register) -- lane gap: zero-register live range across a loop that clobbers bl
// Leaf: ?WatchForQuestCardConditions@CV_BeardyBaldyScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E50910, 1328 bytes, plain `ret`. Ego_r.pdb ?WatchForQuestCardConditions@CV_BeardyBaldyScript@NScript@@QAEXXZ;
//   control flow = the FSE Lua port refs/script_recovery/seed_corpus/sources/BeardyBaldy/FSE/BeardyBaldy/BeardyBaldy.lua (WatchForQuestCardConditions).
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CBaseClass() 0x0099A2D0, IsActiveThreadTerminating 0x00CB7940,
//   GoTalkToBarber 0x00E50E40 (landed under the wrong label GoTalkToBeardyBaldy: NAME_FIXES.tsv), GoTalkToBeardyBaldy 0x00E50650,
//   CScriptThing::operator= 0x008AB980, ~CScriptThing 0x004AA840 / inline, CBasicString<char>::Compare 0x00411570; interface slots 7, 22
//   MsgOnRegionLoaded, 72 GetThingWithScriptName, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective, 655 GetActiveQuestName; thing slots
//   27 MsgIsTalkedToBy, 75 IsAlive. Strings 0x012E5B58, 0x012E5B30, 0x012E5B08, 0x012E5AB4, 0x012E5AD8, 0x0125D1C8, 0x0122D70E.
// Layout: CV_BeardyBaldyScript members at Ego_r.pdb - 0x14 (the landed OnPersist 0x00E4F7F0 list); Phase1RequirementsComplete +0x52,
//   Phase3RequirementsComplete +0x54, AcquiredNewHairdo/Beard/Tash +0x64..0x66. The unused first CCharString local is retail's (ctor 0x00E50918).
#include <string.h>
#pragma intrinsic(strcmp)
template <class T> class CBasicString { public:                       // Ego_r.pdb: the string base of CBasicStringChar (data and inline comparisons)
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (Ego_r.pdb ?Compare@?$CBasicString@D@@KIJPBD0@Z, protected static)
public:
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }
};
#pragma pack(push, 1)
struct CBasicStringChar : public CBasicString<char> {                       // CBasicString<char>, 13 bytes
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing() { }
 CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsTalkedToBy(const CCharString& talker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsTalkedToBy(talker); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFinished;                                 // +0x48
    bool QuestComplete;                                 // +0x49
    bool QuestActivated;                                // +0x4a
    long QuestPhase;                                    // +0x4c
    bool InitialisePhase;                               // +0x50
    bool IntroComplete;                                 // +0x51
    bool Phase1RequirementsComplete;                    // +0x52
    bool Phase2RequirementsComplete;                    // +0x53
    bool Phase3RequirementsComplete;                    // +0x54
    CCharString RequiredHairdo;                         // +0x58
    CCharString RequiredBeard;                          // +0x5c
    CCharString RequiredTash;                           // +0x60
    bool AcquiredNewHairdo;                             // +0x64
    bool AcquiredNewBeard;                              // +0x65
    bool AcquiredNewTash;                               // +0x66
    bool AllHairChanged;                                // +0x67
    bool BeardyBaldyKilled;                             // +0x68
    bool BarberLadyKilled;                              // +0x69
    bool BeardyBaldyLeft;                               // +0x6a
    long IncorrectHairComboCount;                       // +0x6c
    bool AttackedByHero;                                // +0x70
    CTimer RandomSpeechTimer;                           // +0x74
    void GoTalkToBarber();                              // 0x00E50E40
    void GoTalkToBeardyBaldy();                         // 0x00E50650
    void WatchForQuestCardConditions();
};
} // namespace NScript
void NScript::CV_BeardyBaldyScript::WatchForQuestCardConditions()
{
    CCharString questName;
    CCharString region;
    CScriptThing beardyBaldy;
    while (!Phase1RequirementsComplete) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", Game->GetActiveQuestName(), true);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "");
    GoTalkToBarber();
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        while (!AcquiredNewHairdo && !AcquiredNewBeard && !AcquiredNewTash) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "");
        GoTalkToBeardyBaldy();
        AcquiredNewHairdo = false;
        AcquiredNewBeard = false;
        AcquiredNewTash = false;
        for (;;) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            while (!Game->MsgOnRegionLoaded(region)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (region == "BowerstoneSlumsWarehouses")
                break;
            Game->NewScriptFrame();
        }
        if (IsActiveThreadTerminating())
            return;
        if (IsActiveThreadTerminating())
            return;
        beardyBaldy = Game->GetThingWithScriptName("BB_BeardyBaldyMan");
        if (beardyBaldy.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            while (!beardyBaldy.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (!Phase3RequirementsComplete) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "");
            }
        }
        GoTalkToBarber();
        Game->NewScriptFrame();
    }
}
