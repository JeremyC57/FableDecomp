// CScriptDef::GetSizeofClass() -- slot 20: sizeof(CScriptDef) = 0x88
// Leaf: ?GetSizeofClass@CScriptDef@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29500, 6 bytes: b888100000c3.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB5C8 slot 20; Anniversary RTTI CScriptDef (string-literal match, validated 257/258); PDB name set has the GetSizeofClass member of CScriptDef; slot layout from named TLC siblings; function referenced by this vtable only; body=mov eax,imm;ret.
//   ?GetSizeofClass@CScriptDef@@UAEKXZ in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??_ECScriptDef@@UAEPAXI@Z, ??1CScriptDef@@UAE@XZ.
// Was: Constant-return member: returns 0x1088. __fastcall this=ecx. (catalog stand-in _gapscan).
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
unsigned long CScriptDef::GetSizeofClass()
{
    return sizeof(CScriptDef);
}
