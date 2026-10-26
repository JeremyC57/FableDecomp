// CQ_DragonBossFightScript::Main() -- the dragon boss fight quest's main: bind the "Dragon", "DBMinion" and "DBSummoner" entity scripts, finish adding scripted entities (locally and in the game), then spawn the DoMission thread
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D255E0, 468 bytes (480 with padding), plain `ret` (vtable slot at 0x012CB714).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::
//   PostAddScriptedEntities 0x00CB8930 (manifest "CTCVillage::OnInitialActivate" is propagated), CSpawnedFuncBase
//   0x00CDD450 (CSpawnedFunc vtable 0x012CB720), CScriptBase::AddSpawnedFunction 0x00CB7E50; the nested entity scripts'
//   static __fastcall Alloc functions CDragon 0x00D257D0, CDBMinion 0x00D25C60, CDBSummoner 0x00D25E30 (manifest
//   "CTriangleList" labels are propagated), DoMission 0x00D26190; the game script interface's PostAddScriptedEntities
//   (slot 64, +0x100). Strings: "Dragon" 0x012CB768, "DBMinion" 0x012CB754, "DBSummoner" 0x012CB748, "DoMission"
//   0x012CB73C, "" 0x0122D70E.
// Layout: CEntityScriptBinding<CQ_DragonBossFightScript> (0x1C bytes, vtable 0x012CB760) as the landed
//   CQ_CinemaTestScript::Main 0x00F018B0: EntityScriptName +4, PParentScript +8, Func +0xC (left unset),
//   ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18.
// Shape: each name string is a temporary guarded by its own bit of one flag word (ebx: 1, 2, 4, 8); the first
//   ClassAllocValid store reuses the 1 just loaded into ebx.
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

class CGameScriptInterface { public:                          // vtable 0x01260F0C
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
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CQ_DragonBossFightScript : public CScriptBase {
public:
    struct CDragon {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_DragonBossFightScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D257D0
    };
    struct CDBMinion {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_DragonBossFightScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D25C60
    };
    struct CDBSummoner {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_DragonBossFightScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00D25E30
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void DoMission();                                   // 0x00D26190
};
void CQ_DragonBossFightScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_DragonBossFightScript>("Dragon", this, &CDragon::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_DragonBossFightScript>("DBMinion", this, &CDBMinion::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_DragonBossFightScript>("DBSummoner", this, &CDBSummoner::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_DragonBossFightScript>("DoMission", 0, this, &CQ_DragonBossFightScript::DoMission));
}
}