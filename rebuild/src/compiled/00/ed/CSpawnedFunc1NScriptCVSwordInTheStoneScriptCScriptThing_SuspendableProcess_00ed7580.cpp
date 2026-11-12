// CSpawnedFunc1<NScript::CV_SwordInTheStoneScript,CScriptThing>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)(CScriptThing)` member on the stored object
//   with a copy of the stored argument Par1, then set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc1@VCV_SwordInTheStoneScript@NScript@@VCScriptThing@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7580, 50 bytes (byte-identical to the landed <CV_ChapelOfEvilScript> instantiation 0x00E6B200).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc1@VCV_SwordInTheStoneScript@NScript@@VCScriptThing@@@@UAEXXZ,
//   slot 4 of the instantiation's vtable 0x012F3374 (queue entry 111 names that vtable ??_7?$CSpawnedFunc1@VCV_SwordInTheStoneScript@NScript@@VCScriptThing@@@@6B@).
//   Was the stand-in CDispatcher_00ed7580::Fire_CDispatcher_00ed7580 over CEvent_00ed7580 / CCountedPointer / CHandler models.
// Shape: engine/CSpawnedFunc1.h's in-class slot 4 body, emitted out of line by an explicit instantiation of the member (as 0x00E6B200);
//   T is the shared engine/CV_SwordInTheStoneScript.h class (complete, single inheritance: Func is the 4-byte member pointer).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CV_SwordInTheStoneScript.h"

template void CSpawnedFunc1<NScript::CV_SwordInTheStoneScript, CScriptThing>::SuspendableProcess();
