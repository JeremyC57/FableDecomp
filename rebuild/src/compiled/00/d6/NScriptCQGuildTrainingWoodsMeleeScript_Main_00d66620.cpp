// CQ_GuildTrainingWoodsMeleeScript::Main() -- the woods melee training quest's main: reset the mission flags, wait for GuildWoods, bind the "ScorpionHome" entity script, finish adding scripted entities, spawn the WatchForTermination and DoMission threads, then wait until the scorpions are dead and mark the mission succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D66620, length 591 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30
//   / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptBase::AddEntityScriptBinding
//   0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450
//   (CSpawnedFunc vtable 0x012D1D30), CScriptBase::AddSpawnedFunction 0x00CB7E50, CScorpionHome::Alloc 0x00D66BA0
//   (binding vtable 0x012D1D4C), WatchForTermination 0x00D66880 (manifest "CQ_HobbeCaveScript::WatchForTermination" is
//   propagated), DoMission 0x00D66CA0 (landed); the game script interface's NewScriptFrame (slot 7), IsLevelLoaded
//   (slot 11), PostAddScriptedEntities (slot 64).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a, ScorpionsAlive +0x4b.
// Shape: the landed CQ_CinemaTestScript::Main 0x00F018B0 model (inline InitialiseVariables, CEntityScriptBinding with its
//   flags, one flag word in ebx guarding the name temporaries) with the level wait before the binding and the
//   scorpion wait after the spawns.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CSpawnedFuncBase { public:                               // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

struct __declspec(novtable) CEntityScriptBindingBase {
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

class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual bool IsLevelLoaded(const CCharString& level) const;   // slot 11 (+0x2c) IsLevelLoaded
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100) PostAddScriptedEntities
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class CQ_GuildTrainingWoodsMeleeScript : public CScriptBase {
public:
    struct CScorpionHome {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsMeleeScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D66BA0
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool                  MissionSucceeded;             // +0x48
    bool                  MissionFailed;                // +0x49
    bool                  MissionOver;                  // +0x4a
    bool                  ScorpionsAlive;               // +0x4b
    void InitialiseVariables()
    {
        ScorpionsAlive = true;
        MissionSucceeded = false;
        MissionFailed = false;
        MissionOver = false;
    }
    void Main();
    void WatchForTermination();                         // 0x00D66880
    void DoMission();                                   // 0x00D66CA0
};
void CQ_GuildTrainingWoodsMeleeScript::Main()
{
    InitialiseVariables();
    while (!GSI->IsLevelLoaded("GuildWoods")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsMeleeScript>("ScorpionHome", this, &CScorpionHome::Alloc, 1));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript>("WatchForTermination", 0, this, &CQ_GuildTrainingWoodsMeleeScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript>("DoMission", 0, this, &CQ_GuildTrainingWoodsMeleeScript::DoMission));
    while (ScorpionsAlive) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    MissionSucceeded = true;
}
}