// CQ_WaspBossScript::Main() -- the wasp menace quest's main: start the reached-wasp-helper timer (120), wait for LookoutPoint, show the tutorial, bind the nine wasp-quest entity scripts, set the objective, send the guild seal map message, wait for PicnicArea, override the music and spawn the WatchForTermination, DoMission (region PicnicArea) and WatchForCutscene threads
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0EA40, length 1919 bytes. Catalog label "CQ_GuildTrainingScript::Main" is
//   propagated: the body sits in CQ_WaspBossScript's vtable (0x012E00EC) next to the landed Init 0x00E0E820 and spawns
//   0x00E0F1D0 (landed as CQ_WaspBossScript::EndMission; spawned here under the name "WatchForTermination"),
//   DoMission 0x00E12580 and WatchForCutscene 0x00E12330 (catalog label propagated).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc
//   vtable 0x012E0104); binding allocators 0x00E109D0 .. 0x00E11CD0 (binding vtable 0x012E01F8);
//   g_pGameScriptInterface 0x0143E8F8 (CTimer::SetTime, slot 89); the game script interface (+0x40) slots 7, 11, 12, 64,
//   141, 274, 296, 376, 655, 696.
// Layout (retail = Ego_r.pdb - 0x14, as the landed Init 0x00E0E820): ReachedWaspHelper (CTimer) +0x58.
// Shape: bindings / spawned functions as the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20; the DoMission
//   thread passes its region straight to AddSpawnedFunction, so the "PicnicArea" temporary is built before the new.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000();
    CCountedPointer<void> PImp;                         // +0x04 Data, +0x08 Info
    virtual ~CScriptThing();                                    // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptDataBase;
class CEntityScriptBase;
class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };
enum EMusicSetType { MUSIC_SET_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_WaspBossScript : public CScriptBase {
public:
#define ALLOC(Name) struct Name { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WaspBossScript& parent, CScriptDataBase* data, const CScriptThing& thing); };
    ALLOC(CGratefulVillagerSpawn)  // 0x00E109D0
    ALLOC(CWaspChaser)             // 0x00E10B20
    ALLOC(CWaspChaseWoman)         // 0x00E10D70
    ALLOC(CWaspAttacker)           // 0x00E11380
    ALLOC(CWaspVictim)             // 0x00E11580
    ALLOC(CFleeingWoman)           // 0x00E0F400
    ALLOC(CWaspHelper)             // 0x00E0F7C0
    ALLOC(CQueenHornet)            // 0x00E11AD0
    ALLOC(CHornetDrone)            // 0x00E11CD0
#undef ALLOC
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x58 - 0x48];          // MissionSucceeded +0x48 .. QuestStartScreened +0x54
    CTimer                ReachedWaspHelper;            // +0x58
    void Main();
    void WatchForTermination();                         // 0x00E0F1D0
    void DoMission();                                   // 0x00E12580
    void WatchForCutscene();                            // 0x00E12330
};
void CQ_WaspBossScript::Main()
{
    ReachedWaspHelper.SetTime(120);
    while (!Game->IsRegionLoaded("LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    Game->GiveHeroTutorial((ETutorialCategory)6);
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("GratefulVillagerSpawn", this, &CGratefulVillagerSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("WaspChaser", this, &CWaspChaser::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("WaspChaseWoman", this, &CWaspChaseWoman::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("WaspAttacker", this, &CWaspAttacker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("WaspVictim", this, &CWaspVictim::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("FleeingWoman", this, &CFleeingWoman::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("WaspHelper", this, &CWaspHelper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("QueenHornet", this, &CQueenHornet::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_WaspBossScript>("HornetDrone", this, &CHornetDrone::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01", "", "HeroGuildComplexInside");
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_028_GUILDSEAL_WASP_MAP", "", true, true);
    while (!Game->IsLevelLoaded("PicnicArea")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->OverrideMusic((EMusicSetType)23, false, false);
    AddSpawnedFunction(new CSpawnedFunc<CQ_WaspBossScript>("WatchForTermination", 0, this, &CQ_WaspBossScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_WaspBossScript>("DoMission", 0, this, &CQ_WaspBossScript::DoMission), "PicnicArea");
    AddSpawnedFunction(new CSpawnedFunc<CQ_WaspBossScript>("WatchForCutscene", 0, this, &CQ_WaspBossScript::WatchForCutscene), CCharString(""));
}
}