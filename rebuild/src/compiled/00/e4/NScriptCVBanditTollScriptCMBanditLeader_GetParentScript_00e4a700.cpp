// NScript::CV_BanditTollScript::CM_BanditLeader::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CM_BanditLeader@CV_BanditTollScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4A700, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e47d50 "M_BanditLeader" in 00e479b0; Alloc 00e4a660; vtable 012e4f94 slot 3; PDB ?GetParentScript@CM_BanditLeader@CV_BanditTollScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e4a700.
//   ?GetParentScript@CM_BanditLeader@CV_BanditTollScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_BanditTollScript.h (shared header: nested class CM_BanditLeader, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CV_BanditTollScript.h"
CScriptBase* NScript::CV_BanditTollScript::CM_BanditLeader::GetParentScript() const
{
    return &ParentClass;
}
