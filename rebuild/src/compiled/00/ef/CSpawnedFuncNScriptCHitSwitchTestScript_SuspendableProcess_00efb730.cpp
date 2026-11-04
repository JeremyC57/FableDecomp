// CSpawnedFunc<NScript::CHitSwitchTestScript>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)()` member on the stored object, then
//   set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCHitSwitchTestScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFB730, 15 bytes: push esi; mov esi,ecx; mov ecx,[esi+38h] (PClass); call [esi+34h] (Func);
//   mov byte [esi+5],1 (StopRunning); pop esi; ret.
// Naming wave 35c: slot 4 of vtable 0x012F6B6C (no RTTI), whose slot 0 0x00EFBB20 is the landed CSpawnedFunc<NScript::CHitSwitchTestScript>::`scalar deleting
//   destructor' (wave 33 / 29); FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc@VCHitSwitchTestScript@NScript@@@@UAEXXZ. The vtable dword is the only reference to 0x00EFB730.
//   Was a stand-in (old file SuspendableProcess_SuspendableProcess_00efb730.cpp, Leaf SuspendableProcess).
// Shape: engine/CSpawnedFunc.h's in-class slot 4 body, emitted out of line by an explicit instantiation of the member; T is
//   complete with single inheritance, so Func is the 4-byte member pointer (`mov ecx,PClass ; call Func`).
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc.h"
namespace NScript {
class CHitSwitchTestScript : public CScriptBase { };
}

template void CSpawnedFunc<NScript::CHitSwitchTestScript>::SuspendableProcess();
