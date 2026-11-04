// CSpawnedFunc1<NScript::CV_AmbushScamScript,CScriptThing>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)(CScriptThing)` member on the stored object
//   with a copy of the stored argument Par1, then set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc1@VCV_AmbushScamScript@NScript@@VCScriptThing@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2E8C0, 50 bytes.
// Naming wave 35c: slot 4 of vtable 0x012E2B00 (no RTTI), whose slot 0 0x00E2EA60 is the landed CSpawnedFunc1<NScript::CV_AmbushScamScript,CScriptThing>::`scalar deleting
//   destructor' (wave 33 / 29); FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc1@VCV_AmbushScamScript@NScript@@VCScriptThing@@@@UAEXXZ. The vtable dword is the only reference to 0x00E2E8C0.
//   Was a stand-in (old file unattributed_FireCDispatcher00e2e8c0_00e2e8c0.cpp, Leaf -).
// Shape: engine/CSpawnedFunc1.h's in-class slot 4 body, emitted out of line by an explicit instantiation of the member; T is
//   complete with single inheritance, so Func is the 4-byte member pointer (`mov ecx,PClass ; call Func`).
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
namespace NScript {
class CV_AmbushScamScript : public CScriptBase { };
}

template void CSpawnedFunc1<NScript::CV_AmbushScamScript, CScriptThing>::SuspendableProcess();
