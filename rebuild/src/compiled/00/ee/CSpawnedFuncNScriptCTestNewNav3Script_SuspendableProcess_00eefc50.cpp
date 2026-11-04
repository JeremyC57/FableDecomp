// CSpawnedFunc<NScript::CTestNewNav3Script>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)()` member on the stored object, then
//   set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCTestNewNav3Script@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEFC50, 15 bytes: push esi; mov esi,ecx; mov ecx,[esi+38h] (PClass); call [esi+34h] (Func);
//   mov byte [esi+5],1 (StopRunning); pop esi; ret.
// Naming wave 35c: slot 4 of vtable 0x012F5D00 (no RTTI), whose slot 0 0x00EEFC60 is the landed CSpawnedFunc<NScript::CTestNewNav3Script>::`scalar deleting
//   destructor' (wave 33 / 29); FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc@VCTestNewNav3Script@NScript@@@@UAEXXZ. The vtable dword is the only reference to 0x00EEFC50.
//   Was a stand-in (old file CSpawnedFuncNScriptCQRansomVictimChiefsHouseScript_SuspendableProcess_00eefc50.cpp, Leaf SuspendableProcess).
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
class CTestNewNav3Script : public CScriptBase { };
}

template void CSpawnedFunc<NScript::CTestNewNav3Script>::SuspendableProcess();
