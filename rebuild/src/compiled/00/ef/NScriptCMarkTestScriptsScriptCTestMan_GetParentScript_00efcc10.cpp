// NScript::CMarkTestScriptsScript::CTestMan::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CTestMan@CMarkTestScriptsScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFCC10, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00efca47 "TestMan" in 00efca00; Alloc 00efcb70; vtable 012f6d54 slot 3; PDB ?GetParentScript@CTestMan@CMarkTestScriptsScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00efcc10.
//   ?GetParentScript@CTestMan@CMarkTestScriptsScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CMarkTestScriptsScript.h (shared header: nested class CTestMan, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CMarkTestScriptsScript.h"
CScriptBase* NScript::CMarkTestScriptsScript::CTestMan::GetParentScript() const
{
    return &ParentClass;
}
