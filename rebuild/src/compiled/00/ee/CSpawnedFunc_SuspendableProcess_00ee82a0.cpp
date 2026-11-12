// CSpawnedFunc<NScript::CCS_OakValeRevisitedScript>::SuspendableProcess() -- run the spawned member function (PClass->*Func)(), then
// set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCCS_OakValeRevisitedScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE82A0, 15 bytes, thiscall.
// Identity: slot 4 of vtable 0x012F53D4, the vtable the landed NScript::CCS_OakValeRevisitedScript::RegisterMain 0x00EE8210 stores
//   into its `new CSpawnedFunc<CCS_OakValeRevisitedScript>` "Main" thread (CSpawnedFuncBase ctor 0x00CDD450, this vtable, Func
//   +0x34 = the Main vcall thunk 0x00CDD440, PClass +0x38); slot 0 is the landed
//   ??_G?$CSpawnedFunc@VCCS_OakValeRevisitedScript@NScript@@@@UAEPAXI@Z 0x00EE82B0. The body is engine/CSpawnedFunc.h's.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptThing.h"
#include "engine/CCS_OakValeRevisitedScript.h"
#include "engine/CSpawnedFunc.h"

template void CSpawnedFunc<NScript::CCS_OakValeRevisitedScript>::SuspendableProcess();
