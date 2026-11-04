// CScriptDef::Copy(const CDefClassBase* other) -- slot 19: *this = the other def (tail call to operator=)
// Leaf: ?Copy@CScriptDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2A240, 5 bytes: e90b000000.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB5C8 slot 19; Anniversary RTTI CScriptDef (string-literal match, validated 257/258); PDB name set has the Copy member of CScriptDef; slot layout from named TLC siblings; function referenced by this vtable only.
//   ?Copy@CScriptDef@@UAEXPBVCDefClassBase@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??_ECScriptDef@@UAEPAXI@Z, ??1CScriptDef@@UAE@XZ.
// Was: Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup. (catalog stand-in _gapscan).
// Class: the shared header engine/CScriptDef.h (includes as the landed includer rebuild/src/compiled/00/d4/NScriptCQGuildTrainingScriptCWillDummy_Main_00d43450.cpp); only this member is graded.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
void CScriptDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CScriptDef*>(other);
}
