// NScript::CQ_BountyHuntScript::TeleportData::TeleportData() -- default-construct the four CCharString members (defName +0, scriptName +0xc,
//   teleportTo +0x10, teleportMkr +0x14); appearanceSeed +4 / health +8 are left uninitialised
// Leaf: ??0TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D15140, length 36 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x4). Shape: `this` in esi, `mov eax,esi`, `ret`.
// Identity (Naming NF194, 2026-11-20, q6346 / q6366 / q6517): Ego_r.pdb + FableWin.pdb ??0TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ;
//   the 24-byte Ego_r record (defName, appearanceSeed, health, scriptName, teleportTo, teleportMkr) fits the body; CBH_GangDeputy2::Main
//   0x00D18DF0 builds its stack TeleportData with it and destroys it with ~TeleportData 0x00CD4550; every landed referencer binds this
//   spelling through engine/CQ_BountyHuntScript.h. Was ??0CMainGameComponentInit@@QAE@XZ on a local model: that symbol is the real
//   CMainGameComponentInit ctor 0x004131A0 (bound by 0x0042EA8F; Ego_r CMainGameComponentInit is 16 bytes of CWideString / CCharString).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_BountyHuntScript.h"

NScript::CQ_BountyHuntScript::TeleportData::TeleportData()
{
}
