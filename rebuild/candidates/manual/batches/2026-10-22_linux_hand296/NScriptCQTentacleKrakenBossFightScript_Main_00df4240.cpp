// NScript::CQ_TentacleKrakenBossFightScript::Main() -- the Kraken boss fight's main: bind the "KrakenMinion", "M_Experience" and "TheKraken" entity scripts, finish adding scripted entities, then spawn the WatchForTermination and DoMission threads
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF4240, 588 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012DE524),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; the nested entity scripts' static __fastcall Alloc functions
//   0x00DF4520 / 0x00DF4820 / 0x00DF4730 (manifest "CTriangleList" labels are propagated); WatchForTermination
//   0x00DF4490, DoMission 0x00DF4E00; the game script interface's PostAddScriptedEntities (slot 64, +0x100).
// Layout: CEntityScriptBinding<CQ_TentacleKrakenBossFightScript> (0x1C bytes, vtable 0x012DE578) as the landed
//   CQ_DragonBossFightScript::Main 0x00D255E0: EntityScriptName +4, PParentScript +8, ClassAllocFunc +0x10,
//   ClassAllocValid +0x14, Flags +0x18 ("TheKraken" is bound with flags 1).
// Shape: each name string is a temporary guarded by its own bit of one flag word (ebx: 1, 2, 4, 8, 0x10); the
//   first ClassAllocValid store reuses the 1 just loaded into ebx. Same template as the dragon fight's Main.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
struct CSpawnedFuncBase {                               // retail 0x34 bytes
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

struct CScriptThing;
struct CScriptDataBase;
struct CEntityScriptBase;
struct CActiveEntityScriptBase;
template <class T> struct CCountedPointer { T* Data; void* Info; };

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

struct CGameScriptInterface {                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
struct CQ_TentacleKrakenBossFightScript : CScriptBase {
    struct CKrakenMinion {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_TentacleKrakenBossFightScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF4520
    };
    struct CM_Experience {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_TentacleKrakenBossFightScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF4820
    };
    struct CTheKraken {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_TentacleKrakenBossFightScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00DF4730
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void WatchForTermination();                         // 0x00DF4490
    void DoMission();                                   // 0x00DF4E00
};
void CQ_TentacleKrakenBossFightScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TentacleKrakenBossFightScript>("KrakenMinion", this, &CKrakenMinion::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TentacleKrakenBossFightScript>("M_Experience", this, &CM_Experience::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_TentacleKrakenBossFightScript>("TheKraken", this, &CTheKraken::Alloc, 1));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_TentacleKrakenBossFightScript>("WatchForTermination", 0, this, &CQ_TentacleKrakenBossFightScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CQ_TentacleKrakenBossFightScript>("DoMission", 0, this, &CQ_TentacleKrakenBossFightScript::DoMission));
}
}
