// CSpawnedFunc<NScript::CQ_RansomVictimChiefsHouseScript>::SuspendableProcess() -- the spawned script thread: call the bound `void (T::*)()` member on the stored object, then
//   set StopRunning
// Leaf: ?SuspendableProcess@?$CSpawnedFunc@VCQ_RansomVictimChiefsHouseScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEBEE0, 15 bytes: push esi; mov esi,ecx; mov ecx,[esi+38h] (PClass); call [esi+34h] (Func);
//   mov byte [esi+5],1 (StopRunning); pop esi; ret.
// Naming wave 35c: slot 4 of vtable 0x012DDBBC (no RTTI), whose slot 0 0x00DEC710 is the landed CSpawnedFunc<NScript::CQ_RansomVictimChiefsHouseScript>::`scalar deleting
//   destructor' (wave 33 / 29); Ego_r.pdb + FableWin.pdb ?SuspendableProcess@?$CSpawnedFunc@VCQ_RansomVictimChiefsHouseScript@NScript@@@@UAEXXZ. The vtable dword is the only reference to 0x00DEBEE0.
//   Was a stand-in (old file CSpawnedFuncNScriptCQRansomVictimChiefsHouseScript_SuspendableProcess_00debee0.cpp, Leaf SuspendableProcess).
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
class CQ_RansomVictimChiefsHouseScript : public CScriptBase { };
}

template void CSpawnedFunc<NScript::CQ_RansomVictimChiefsHouseScript>::SuspendableProcess();
