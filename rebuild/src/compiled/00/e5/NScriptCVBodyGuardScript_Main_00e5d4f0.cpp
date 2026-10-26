// CV_BodyGuardScript::Main() -- bind the bodyguard's five entity scripts, then spawn HenchmanDeductWages, HenchmanCommentary, FisticuffsWatch and CheckForHenchmanFiredOverride
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5D4F0, length 1024 bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 /
//   ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012E6E60); the entity
//   scripts' static __fastcall Alloc functions CBodyGuard 0x00E5DD60, CHenchmanComment 0x00E5F1C0,
//   CHenchmanRegionEntranceComment 0x00E5F5C0, CHenchmanRegionExitComment 0x00E5F800, CBodyGuardRespawn 0x00E5DFF0 (binding
//   vtable 0x012E7004); the spawned HenchmanDeductWages 0x00E5D9A0, HenchmanCommentary 0x00E5E350, FisticuffsWatch
//   0x00E5DB30, CheckForHenchmanFiredOverride 0x00E5D900; GameCalls slot 64 PostAddScriptedEntities.
// Layout: CEntityScriptBinding (0x1C bytes) and CSpawnedFunc (0x3C) as the landed CV_KnotholeGladeGatesScript::Main
//   0x00E9FCD0; every binding passes Flags 0; the spawned functions run in the "" region.
// Shape: name strings inside new-expressions are conditional temporaries (one ebx flag bit each); the region name is an
//   explicit CCharString("") argument, so it is built after the new-expression.
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

class CGameScriptInterface { public:                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;                                  // slot 64 (+0x100)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
};
class CV_BodyGuardScript : public CScriptBase {
public:
    struct CBodyGuard { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00E5DD60
    struct CHenchmanComment { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00E5F1C0
    struct CHenchmanRegionEntranceComment { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00E5F5C0
    struct CHenchmanRegionExitComment { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00E5F800
    struct CBodyGuardRespawn { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00E5DFF0
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void HenchmanDeductWages();                         // 0x00E5D9A0
    void HenchmanCommentary();                          // 0x00E5E350
    void FisticuffsWatch();                             // 0x00E5DB30
    void CheckForHenchmanFiredOverride();               // 0x00E5D900
    void Main();
};
void CV_BodyGuardScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("BodyGuard", this, &CBodyGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("HenchmanComment", this, &CHenchmanComment::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("HenchmanRegionEntranceComment", this, &CHenchmanRegionEntranceComment::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("HenchmanRegionExitComment", this, &CHenchmanRegionExitComment::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("M_BodyGuardRespawn", this, &CBodyGuardRespawn::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("HenchmanDeductWages", 0, this, &CV_BodyGuardScript::HenchmanDeductWages), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("HenchmanCommentary", 0, this, &CV_BodyGuardScript::HenchmanCommentary), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("FisticuffsWatch", 0, this, &CV_BodyGuardScript::FisticuffsWatch), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("CheckForHenchmanFiredOverride", 0, this, &CV_BodyGuardScript::CheckForHenchmanFiredOverride), CCharString(""));
}
}