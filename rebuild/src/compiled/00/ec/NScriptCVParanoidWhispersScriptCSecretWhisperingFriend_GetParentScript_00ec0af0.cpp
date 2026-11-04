// NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CSecretWhisperingFriend@CV_ParanoidWhispersScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC0AF0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00ebf030 "SecretWhisperingFriend" in 00ebef80; Alloc 00ec0a50; vtable 012f11f8 slot 3; PDB ?GetParentScript@CSecretWhisperingFriend@CV_ParanoidWhispersScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ec0af0.
//   ?GetParentScript@CSecretWhisperingFriend@CV_ParanoidWhispersScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_ParanoidWhispersScript.h (shared header: nested class CSecretWhisperingFriend, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_ParanoidWhispersScript.h"
CScriptBase* NScript::CV_ParanoidWhispersScript::CSecretWhisperingFriend::GetParentScript() const
{
    return &ParentClass;
}
