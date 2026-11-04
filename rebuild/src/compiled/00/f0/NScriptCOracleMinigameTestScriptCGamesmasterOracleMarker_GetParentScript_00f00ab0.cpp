// NScript::COracleMinigameTestScript::CGamesmasterOracleMarker::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CGamesmasterOracleMarker@COracleMinigameTestScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F00AB0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00effebe "GamesmasterOracleMarker" in 00effe10; Alloc 00f00a10; vtable 012f7238 slot 3; PDB ?GetParentScript@CGamesmasterOracleMarker@COracleMinigameTestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00f00ab0.
//   ?GetParentScript@CGamesmasterOracleMarker@COracleMinigameTestScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/COracleMinigameTestScript.h (shared header: nested class CGamesmasterOracleMarker, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/COracleMinigameTestScript.h"
CScriptBase* NScript::COracleMinigameTestScript::CGamesmasterOracleMarker::GetParentScript() const
{
    return &ParentClass;
}
