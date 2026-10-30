// CQ_AmbushTradersScript::DoQuest() -- the quest body: wait for the bandit contact intro, point the quest card at Bowerstone Jail, wait until the hero enters Bowerstone Jail by the entrance from Bowerstone Posh, activate phase 1, spawn the WatchForRegionLeaving / WatchForBanditsDeath / IntroPart2Checker threads, collect the bandit allies, play the intro, set the next objective, show the quest start screen, run phases one and two and, when the quest finished without the contact being killed, wait for Bowerstone Posh to load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0B130, length 1606 bytes.
// Callees: CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, Compare 0x00411570
//   (the inlined CCharString == const char*), CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, operator new 0x00BFEA1A, operator
//   delete 0x00BFE9BC, CScriptThing::operator= 0x008AB980 / GetDataString 0x004AA900 / ~CScriptThing 0x004AA840,
//   CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc vtable 0x012C6CC0), CScriptBase::IsActiveThreadTerminating 0x00CB7940 /
//   AddSpawnedFunction 0x00CB7E50; WatchForRegionLeaving 0x00F0B810, WatchForBanditsDeath 0x00F0B960 (landed),
//   IntroPart2Checker 0x00F0BA50, Intro 0x00F0BB10, PhaseOne 0x00F0B780 (landed), PhaseTwo 0x00F0C140; the game script
//   interface (+0x40) slots 7, 12, 22, 70, 75, 81, 276, 296, 309, 655.
// Layout (retail = Ego_r.pdb - 0x14, -4 per preceding vector): QuestFailed +0x48, QuestFinished +0x49,
//   BanditContactIntroComplete +0x67, BanditContactKilledBeforeQuestEnd +0x68, AllBandits +0xa8.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement); spawned-function model of
//   the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620; inline CCharString == of the landed CQ_BountyHuntScript
//   0x00D151D0. The entrance thing's destructor is inline at the two terminating returns that bracket the region wait and
//   out of line (0x004AA840) everywhere else (in-body inline_depth(0), as the landed CMeleeThunder::Main 0x00D58080).
//   The region-entrance wait is `for (;;) { if (terminating) return; ... }` (VC duplicates the head test after the
//   NewScriptFrame and both share one inline destructor expansion); the GetNearestWithDefName temporary is destroyed out
//   of line (its statement sits under inline_depth(0)). SpawnFunction and the CSpawnedFunc ctor are __forceinline: past
//   the inline destructors VC7.1's per-function inline budget is spent and plain inline copies are called out of line.
//   The QuestFailed branch holds only the terminating test (its tail is merged with the region-wait branch's).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    __forceinline ~CScriptThing() {}                    // out-of-line copy 0x004AA840
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
public:
    virtual CCharString GetDataString() const;   // 0x004AA900
};
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
using std::vector;
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
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
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool                  QuestFailed;                  // +0x48
    bool                  QuestFinished;                // +0x49
    unsigned char         _pad4a[0x67 - 0x4a];
    bool                  BanditContactIntroComplete;   // +0x67
    bool                  BanditContactKilledBeforeQuestEnd;   // +0x68
    unsigned char         _pad69[0xa8 - 0x69];
    vector<CScriptThing>  AllBandits;                   // +0xa8
    void DoQuest();
    void WatchForRegionLeaving();                       // 0x00F0B810
    void WatchForBanditsDeath();                        // 0x00F0B960
    void IntroPart2Checker();                           // 0x00F0BA50
    void Intro();                                       // 0x00F0BB10
    void PhaseOne();                                    // 0x00F0B780
    void PhaseTwo();                                    // 0x00F0C140
};
void CQ_AmbushTradersScript::DoQuest()
{
    CCharString region;
    while (!BanditContactIntroComplete) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_02", "BowerstoneJail", "BowerstoneSlums");
    CScriptThing entrance;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
#pragma inline_depth(0)
        entrance = Game->GetNearestWithDefName(Game->GetHero(), "REGION_ENTRANCE_POINT");
#pragma inline_depth()
        if (region == "BowerstoneJail" && entrance.GetDataString() == "FromBowerstonePosh")
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    Game->ActivateQuest("Q_AmbushTraders_ActivatePhase1");
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForRegionLeaving", 0, this, &CQ_AmbushTradersScript::WatchForRegionLeaving), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForBanditsDeath", 0, this, &CQ_AmbushTradersScript::WatchForBanditsDeath), CCharString(""));
    Game->GetAllThingsWithScriptName("AT_BanditAlly", AllBandits);
    AddSpawnedFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("IntroPart2Checker", 0, this, &CQ_AmbushTradersScript::IntroPart2Checker), CCharString(""));
    Intro();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_03", "DesertedFarm", "BowerstoneSlums");
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    PhaseOne();
    PhaseTwo();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    if (QuestFailed) {
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    } else if (QuestFinished && !BanditContactKilledBeforeQuestEnd) {
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        while (!Game->IsRegionLoaded("BowerstonePosh")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
        }
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}
}