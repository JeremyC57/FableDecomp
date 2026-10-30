// NScript::CEmptyScript::Main() -- the empty script: spawn Bum, then idle one script frame at a time until the thread terminates
// Leaf: ?Main@CEmptyScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEE800, 170 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the RTTI-less level-script vtable 0x012F5954, which the landed constructor NScript::CEmptyScript::CEmptyScript 0x00EEE730 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Main@CEmptyScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CEmptyScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: operator new 0x00BFEA1A (0x3C), CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450 (CSpawnedFunc<CEmptyScript>
//   vtable 0x012F5968, Func Bum 0x00EEE8B0, name "Bum" 0x012F5984 a conditional temporary), AddSpawnedFunction 0x00CB7E50 (""
//   0x0122D70E), IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / 0x0099EAE0; interface slot 7 NewScriptFrame.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
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
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                            // slot 7 (+0x1c)
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
class CEmptyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void Bum();                                         // 0x00EEE8B0
    virtual void Main();
};

void CEmptyScript::Main()
{
    SpawnFunction(new CSpawnedFunc<CEmptyScript>("Bum", 0, this, &CEmptyScript::Bum));
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
}