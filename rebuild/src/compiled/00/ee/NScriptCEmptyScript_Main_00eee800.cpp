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
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
void* operator new(unsigned int size);                  // 0x00BFEA1A
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CEmptyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void Bum();                                         // 0x00EEE8B0
    virtual void Main();
};

void CEmptyScript::Main()
{
    AddSpawnedFunction(new CSpawnedFunc<CEmptyScript>("Bum", 0, this, &CEmptyScript::Bum), CCharString(""));
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
}