// NScript::CQ_AmbushTradersScript::TeleportData::TeleportData() -- two empty strings (defName +0x00, scriptName +0x0c) and the empty
//   STLport vector<CCharString> itemsCarried (+0x10..+0x18 zeroed); appearanceSeed +0x04 / health +0x08 left unset
// Leaf: ??0TeleportData@CQ_AmbushTradersScript@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0D470, length 31 bytes (+ int3 padding).
// Callee: CCharString::CCharString() @ 0x0099E4B0 (twice). The one caller is CAT_ConvoyMember::Main (0x00F0CAA0, @ 0x00F0D1AD), which
//   builds this record on its stack and pushes it onto TeleportInfo (push_back 0x00F0D490).
// Identity (Naming NF194, 2026-11-20, q6345 / q6365 / q6414): Ego_r.pdb + FableWin.pdb ??0TeleportData@CQ_AmbushTradersScript@NScript@@QAE@XZ;
//   the Ego_r record (defName, appearanceSeed, health, scriptName, itemsCarried) fits the body (CCharString ctors at +0 / +0xc, the
//   vector's three words zeroed); the landed caller 0x00F0CAA0 already binds this spelling. Was the stand-in SScriptQuery_00f0d470
//   (??0SScriptQuery_00f0d470@@QAE@XZ) on a local model; now the class from engine/CQ_AmbushTradersScript.h
//   (FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_AmbushTradersScript.h"

NScript::CQ_AmbushTradersScript::TeleportData::TeleportData()
{
}
