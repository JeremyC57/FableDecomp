// NScript::CHeroTurningDebuggerScript::Main() -- the hero-turning debugger: post-add the scripted entities and spawn MoveHeroTest
// Leaf: ?Main@CHeroTurningDebuggerScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFC260, 155 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the RTTI-less level-script vtable 0x012F6C58, which the landed constructor NScript::CHeroTurningDebuggerScript::CHeroTurningDebuggerScript 0x00EFC190 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Main@CHeroTurningDebuggerScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CHeroTurningDebuggerScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptBase::PostAddScriptedEntities 0x00CB8930, operator new 0x00BFEA1A (0x3C), CSpawnedFuncBase(const CCharString&,
//   unsigned long) 0x00CDD450 (CSpawnedFunc<CHeroTurningDebuggerScript> vtable 0x012F6C6C, Func MoveHeroTest 0x00EFC300, name
//   "MoveHeroTest" 0x012F6C88 a conditional temporary), AddSpawnedFunction 0x00CB7E50 ("" 0x0122D70E), CCharString 0x0099EBF0 /
//   0x0099EAE0; interface slot 64 PostAddScriptedEntities.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSpawnedFuncBase {                                // retail 0x34 bytes (the landed CQR_EscortTraderScript::Main 0x00E1E110 model)
public:
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> class CSpawnedFunc : public CSpawnedFuncBase {   // 0x3c bytes
public:
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) { }
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
void* operator new(unsigned int size);                  // 0x00BFEA1A
class CGameScriptInterface {                     // the script interface vtable (slot names: ghidra_out/cgsi_slots_aligned.tsv)
public:
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
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void PostAddScriptedEntities();                     // 0x00CB8930
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CHeroTurningDebuggerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void MoveHeroTest();                                // 0x00EFC300
    virtual void Main();
};

void CHeroTurningDebuggerScript::Main()
{
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CHeroTurningDebuggerScript>("MoveHeroTest", 0, this, &CHeroTurningDebuggerScript::MoveHeroTest));
}
}