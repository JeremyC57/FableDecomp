// NScript::CQ_SecretPassageScript::InitialiseVariables() -- clear the quest flags: MissionSucceeded, MissionFailed, MissionOver, MotherFound, DoorsBlocked, DoorsBeingDone and QuestStartScreenShown (OkayToSpawn is left alone)
// Leaf: ?InitialiseVariables@CQ_SecretPassageScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DED0A0, 24 bytes: xor al,al; mov [ecx+48h..4Dh],al; mov [ecx+4Fh],al; ret (thiscall).
// Identity (two sources): FableWin.pdb ?InitialiseVariables@CQ_SecretPassageScript@NScript@@QAEXXZ (public, non-virtual); its only caller is the
//   landed CQ_SecretPassageScript::Main 0x00DECAD0, which calls it first on `this` and binds this spelling through the shared header; the body
//   writes exactly the header's bool flags +0x48..+0x4D and +0x4F (retail = Ego_r.pdb - 0x14), skipping OkayToSpawn +0x4E.
//   Name-fix NF186 (q6215): was the propagated byte-twin label CMeleeApprentice::Init (free __fastcall CMeleeApprentice_Init on a local struct).
// Class: engine/NScript_CQ_SecretPassageScript.h (shared header; declares InitialiseVariables).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/NScript_CQ_SecretPassageScript.h"
namespace NScript {
void CQ_SecretPassageScript::InitialiseVariables()
{
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    MotherFound = false;
    DoorsBlocked = false;
    DoorsBeingDone = false;
    QuestStartScreenShown = false;
}
}
