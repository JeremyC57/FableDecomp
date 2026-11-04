// NScript::CV_DemonDoorsScript::CDF_DoDamage::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CDF_DoDamage@CV_DemonDoorsScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E70B90, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e6e621 "DF_DoDamage" in 00e6e380; Alloc 00e70af0; vtable 012e8f2c slot 3; PDB ?GetParentScript@CDF_DoDamage@CV_DemonDoorsScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e70b90.
//   ?GetParentScript@CDF_DoDamage@CV_DemonDoorsScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_DemonDoorsScript.h (shared header: nested class CDF_DoDamage, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_DemonDoorsScript.h"
CScriptBase* NScript::CV_DemonDoorsScript::CDF_DoDamage::GetParentScript() const
{
    return &ParentClass;
}
