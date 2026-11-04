// NScript::CV_ParanoidWhispersScript::CPW_NecklaceDiggingSpot::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CPW_NecklaceDiggingSpot@CV_ParanoidWhispersScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC09A0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00ebf098 "PW_NecklaceDiggingSpot" in 00ebef80; Alloc 00ec0900; vtable 012f11dc slot 3; PDB ?GetParentScript@CPW_NecklaceDiggingSpot@CV_ParanoidWhispersScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ec09a0.
//   ?GetParentScript@CPW_NecklaceDiggingSpot@CV_ParanoidWhispersScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_ParanoidWhispersScript.h (shared header: nested class CPW_NecklaceDiggingSpot, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_ParanoidWhispersScript.h"
CScriptBase* NScript::CV_ParanoidWhispersScript::CPW_NecklaceDiggingSpot::GetParentScript() const
{
    return &ParentClass;
}
