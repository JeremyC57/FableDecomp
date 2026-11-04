// CSpawnedFunc<NScript::CV_LostTraderScript>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)()` member on the stored object, then
//   set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCV_LostTraderScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA2E70, 15 bytes: push esi; mov esi,ecx; mov ecx,[esi+38h] (PClass); call [esi+34h] (Func);
//   mov byte [esi+5],1 (StopRunning); pop esi; ret.
// Naming wave 35c: slot 4 of vtable 0x012EDCFC (no RTTI), whose slot 0 0x00EA58E0 is the landed CSpawnedFunc<NScript::CV_LostTraderScript>::`scalar deleting
//   destructor' (wave 33 / 29); FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc@VCV_LostTraderScript@NScript@@@@UAEXXZ. The vtable dword is the only reference to 0x00EA2E70.
//   Was a stand-in (old file CSpawnedFuncNScriptCQRansomVictimChiefsHouseScript_SuspendableProcess_00ea2e70.cpp, Leaf SuspendableProcess).
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
class CV_LostTraderScript : public CScriptBase { };
}

template void CSpawnedFunc<NScript::CV_LostTraderScript>::SuspendableProcess();
