// CSpawnedFunc1<NScript::CV_BookCollectingScript,long>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)(long)` member on the stored object
//   with a copy of the stored argument Par1, then set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc1@VCV_BookCollectingScript@NScript@@J@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E566D0, 19 bytes.
// Naming wave 35c: slot 4 of vtable 0x012E6228 (no RTTI), whose slot 0 0x00E57000 is the landed CSpawnedFunc1<NScript::CV_BookCollectingScript,long>::`scalar deleting
//   destructor' (wave 33 / 29); FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc1@VCV_BookCollectingScript@NScript@@J@@UAEXXZ. The vtable dword is the only reference to 0x00E566D0.
//   Was a stand-in (old file CSpawnedFunc1_SuspendableProcess_00e566d0.cpp, Leaf -).
// Shape: engine/CSpawnedFunc1.h's in-class slot 4 body, emitted out of line by an explicit instantiation of the member; T is
//   complete with single inheritance, so Func is the 4-byte member pointer (`mov ecx,PClass ; call Func`).
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
namespace NScript {
class CV_BookCollectingScript : public CScriptBase { };
}

template void CSpawnedFunc1<NScript::CV_BookCollectingScript, long>::SuspendableProcess();
