// CV_FisticuffsClubScript::Main() -- the Fisticuffs Club quest main: initialise the state, bind the FisticuffsClubRing / FisticuffsMember / Tyler / RingFighter entity scripts and spawn the KeepVariablesUpToDate and ActivateFisticuffsAtNight threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82220, 0x2ab bytes, plain `ret`.
// Callees: InitialiseVariables 0x00E82650, operator new 0x00BFEA1A, CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_FisticuffsClubScript> vtable 0x012EAF28), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012EAFEC), CScriptBase::PostAddScriptedEntities 0x00CB8930
//   (manifest "CTCVillage::OnInitialActivate" is propagated); allocators 0x00E82870 / 0x00E838A0 / 0x00E83570 / 0x00E83780;
//   threads KeepVariablesUpToDate 0x00E824E0, ActivateFisticuffsAtNight 0x00E83D50; CGameScriptInterface slot 64.
// Shape: CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 binding model; the spawns use the inline SpawnFunction
//   (region temporary built after operator new).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
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
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
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

class CGameScriptInterface;
class CGameScriptInterface { public:                       // retail vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities(void) const;   // slot 64 (+0x100) 0x006E7460
};
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_FisticuffsClubScript : public CScriptBase {
public:
    struct CFisticuffsClubRing {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E82870
    };
    struct CFisticuffsMember {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E838A0
    };
    struct CTyler {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E83570
    };
    struct CRingFighter {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E83780
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void InitialiseVariables();                          // 0x00E82650
    void KeepVariablesUpToDate();                        // 0x00E824E0
    void ActivateFisticuffsAtNight();                    // 0x00E83D50
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::Main()
{
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("FisticuffsClubRing", this, &CFisticuffsClubRing::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("FisticuffsMember", this, &CFisticuffsMember::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("Tyler", this, &CTyler::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("RingFighter", this, &CRingFighter::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_FisticuffsClubScript>("KeepVariablesUpToDate", 0, this, &NScript::CV_FisticuffsClubScript::KeepVariablesUpToDate));
    SpawnFunction(new CSpawnedFunc<CV_FisticuffsClubScript>("ActivateFisticuffsAtNight", 0, this, &NScript::CV_FisticuffsClubScript::ActivateFisticuffsAtNight));
}