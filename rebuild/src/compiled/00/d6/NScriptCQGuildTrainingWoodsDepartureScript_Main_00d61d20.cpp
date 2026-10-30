// CQ_GuildTrainingWoodsDepartureScript::Main() -- the woods departure quest's main: reset the mission flags, wait for GuildWoods, bind the ArtifactThief / FinalMaze / ScorpionHome entity scripts, finish adding scripted entities, spawn the WatchForTermination and DoMission threads, set the quest-card objective and create the maze tutorial rival hero at the maze creation marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D61D20, length 1047 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30
//   / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptBase::AddEntityScriptBinding
//   0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450
//   (CSpawnedFunc vtable 0x012D1610), CScriptBase::AddSpawnedFunction 0x00CB7E50, ~CScriptThing 0x004AA840; binding
//   allocators 0x00D62360 (ArtifactThief), 0x00D62270 (FinalMaze), 0x00D63CC0 (ScorpionHome) (binding vtable 0x012D16A8);
//   WatchForTermination 0x00D62140, DoMission 0x00D63DC0 (landed); the game script interface (+0x40) slots 7, 11, 64, 72,
//   91, 296; CScriptThing::GetPos (slot 6).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a, DepartureMissionPoint +0x4c.
// Shape: the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 model (one flag word guarding the name temporaries).
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
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                    // 0x004AA840
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQ_GuildTrainingWoodsDepartureScript : public CScriptBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAlloc)(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing);
    struct CArtifactThief { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D62360
    struct CFinalMaze { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing); };       // 0x00D62270
    struct CScorpionHome { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing); };    // 0x00D63CC0
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool                  MissionSucceeded;             // +0x48
    bool                  MissionFailed;                // +0x49
    bool                  MissionOver;                  // +0x4a
    long                  DepartureMissionPoint;        // +0x4c
    void InitialiseVariables()
    {
        MissionSucceeded = false;
        MissionFailed = false;
        MissionOver = false;
    }
    void Main();
    void WatchForTermination();                         // 0x00D62140
    void DoMission();                                   // 0x00D63DC0
};
void CQ_GuildTrainingWoodsDepartureScript::Main()
{
    InitialiseVariables();
    while (!GSI->IsLevelLoaded("GuildWoods")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsDepartureScript>("ArtifactThief", this, &CArtifactThief::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsDepartureScript>("FinalMaze", this, &CFinalMaze::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsDepartureScript>("ScorpionHome", this, &CScorpionHome::Alloc, 1));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript>("WatchForTermination", 0, this, &CQ_GuildTrainingWoodsDepartureScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript>("DoMission", 0, this, &CQ_GuildTrainingWoodsDepartureScript::DoMission));
    DepartureMissionPoint = 0;
    GSI->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "");
    GSI->CreateCreature("CREATURE_RIVAL_HERO_MAZE_TUTORIAL", GSI->GetThingWithScriptName("MazeCreationMarker").GetPos(), "FinalMaze", false);
}
}