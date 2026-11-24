// CSpawnedFunc<NScript::CV_HauntedHouseScript>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)()` member on the stored object, then
//   set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCV_HauntedHouseScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93E30, 15 bytes: push esi; mov esi,ecx; mov ecx,[esi+38h] (PClass); call [esi+34h] (Func);
//   mov byte [esi+5],1 (StopRunning); pop esi; ret.
// Naming wave 35c: slot 4 of vtable 0x012ECBA8 (no RTTI), whose slot 0 0x00E93E40 is the landed CSpawnedFunc<NScript::CV_HauntedHouseScript>::`scalar deleting
//   destructor' (wave 33 / 29); FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc@VCV_HauntedHouseScript@NScript@@@@UAEXXZ. The vtable dword is the only reference to 0x00E93E30.
//   Was a stand-in (old file CSpawnedFuncNScriptCQRansomVictimChiefsHouseScript_SuspendableProcess_00e93e30.cpp, Leaf SuspendableProcess).
// Shape: engine/CSpawnedFunc.h's in-class slot 4 body, emitted out of line by an explicit instantiation of the member; T is
//   complete with single inheritance, so Func is the 4-byte member pointer (`mov ecx,PClass ; call Func`).
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

template void CSpawnedFunc<NScript::CV_HauntedHouseScript>::SuspendableProcess();
