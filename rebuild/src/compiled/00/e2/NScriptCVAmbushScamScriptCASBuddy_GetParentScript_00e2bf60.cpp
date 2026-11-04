// NScript::CV_AmbushScamScript::CAS_Buddy::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CAS_Buddy@CV_AmbushScamScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2BF60, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e2b1b8 "AS_Buddy" in 00e2b090; Alloc 00e2bec0; vtable 012e286c slot 3; PDB ?GetParentScript@CAS_Buddy@CV_AmbushScamScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e2bf60.
//   ?GetParentScript@CAS_Buddy@CV_AmbushScamScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_AmbushScamScript.h (shared header: nested class CAS_Buddy, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CV_AmbushScamScript.h"
CScriptBase* NScript::CV_AmbushScamScript::CAS_Buddy::GetParentScript() const
{
    return &ParentClass;
}
