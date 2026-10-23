// CV_ChickenKickingScript::Main() -- the Chicken Kicking quest main: wait for the OakBay region to load, bind the ChickenMaster / ChickenSign / KickedChicken / Spectator entity scripts and spawn the CreateSpectators and LookAfterOrganiser threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E62A00, 0x347 bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_ChickenKickingScript> vtable 0x012E732C), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012E73C8), CScriptBase::PostAddScriptedEntities 0x00CB8930;
//   allocators 0x00E634B0 / 0x00E64D10 / 0x00E63590 / 0x00E63660 (all bound with flags 1); threads CreateSpectators
//   0x00E62D60, LookAfterOrganiser 0x00E631F0; CGameScriptInterface slots 7, 12 IsRegionLoaded, 64.
// Shape: same as CV_SingingStonesScript::Main 0x00ED10A0 (WAIT_UNTIL, binding model, inline SpawnFunction).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
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
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
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

struct CGameScriptInterface;
struct CGameScriptInterface {                       // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame(void) const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(CCharString const &) const;   // slot 12 (+0x30) 0x0088E300
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063();
    virtual void PostAddScriptedEntities(void) const;   // slot 64 (+0x100) 0x006E7460
};
struct CScriptBase {                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
struct CV_ChickenKickingScript : CScriptBase {
    struct CChickenMaster {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E634B0
    };
    struct CChickenSign {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E64D10
    };
    struct CKickedChicken {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E63590
    };
    struct CSpectator {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E63660
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void CreateSpectators();                             // 0x00E62D60
    void LookAfterOrganiser();                           // 0x00E631F0
};

void CV_ChickenKickingScript::Main()
{
    while (!Game->IsRegionLoaded("OakBay")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("ChickenMaster", this, &CChickenMaster::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("ChickenSign", this, &CChickenSign::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("KickedChicken", this, &CKickedChicken::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("Spectator", this, &CSpectator::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_ChickenKickingScript>("CreateSpectators", 0, this, &CV_ChickenKickingScript::CreateSpectators));
    SpawnFunction(new CSpawnedFunc<CV_ChickenKickingScript>("LookAfterOrganiser", 0, this, &CV_ChickenKickingScript::LookAfterOrganiser));
}
