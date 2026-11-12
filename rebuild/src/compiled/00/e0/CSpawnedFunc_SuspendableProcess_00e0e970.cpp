// CSpawnedFunc<NScript::CQ_WaspBossScript>::SuspendableProcess() -- run the spawned member function (PClass->*Func)(), then set
// StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCQ_WaspBossScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E970, 15 bytes, thiscall.
// Identity: slot 4 of vtable 0x012E0104, the vtable NScript::CQ_WaspBossScript::RegisterMain 0x00E0E8E0, Main 0x00E0EA40 and DoMission
//   0x00E12580 store into their `new CSpawnedFunc<CQ_WaspBossScript>` threads (CSpawnedFuncBase ctor 0x00CDD450, then this vtable,
//   Func +0x34, PClass +0x38); slot 0 is the landed ??_G?$CSpawnedFunc@VCQ_WaspBossScript@NScript@@@@UAEPAXI@Z 0x00E0E980, slots
//   1..3 the CSuspendableProcessMicrothread virtuals, 5 / 6 CSpawnedFuncBase's. CASuspendableProcess::SuspendableProcess is slot 4
//   (engine/CASuspendableProcess.h); the body is engine/CSpawnedFunc.h's (176 landed instantiations share it). Ego_r.pdb /
//   FableWin.pdb name CSpawnedFunc<T>::SuspendableProcess for the script classes.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/NScript_CQ_WaspBossScript.h"
#include "engine/CSpawnedFunc.h"

template void CSpawnedFunc<NScript::CQ_WaspBossScript>::SuspendableProcess();
