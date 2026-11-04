// NScript::CV_SickChildScript::CSickChildsSister::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CSickChildsSister@CV_SickChildScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC6850, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00ec55f1 "SickChildsSister" in 00ec54e0; Alloc 00ec67b0; vtable 012f1964 slot 3; PDB ?GetParentScript@CSickChildsSister@CV_SickChildScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ec6850.
//   ?GetParentScript@CSickChildsSister@CV_SickChildScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_SickChildScript.h (shared header: nested class CSickChildsSister, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_SickChildScript.h"
CScriptBase* NScript::CV_SickChildScript::CSickChildsSister::GetParentScript() const
{
    return &ParentClass;
}
