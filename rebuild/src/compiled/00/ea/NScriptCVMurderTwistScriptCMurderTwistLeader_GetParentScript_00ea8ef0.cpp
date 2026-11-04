// NScript::CV_MurderTwistScript::CMurderTwistLeader::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CMurderTwistLeader@CV_MurderTwistScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8EF0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00ea887c "MurderTwistLeader" in 00ea8830; Alloc 00ea8e50; vtable 012ee758 slot 3; PDB ?GetParentScript@CMurderTwistLeader@CV_MurderTwistScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00ea8ef0.
//   ?GetParentScript@CMurderTwistLeader@CV_MurderTwistScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_MurderTwistScript.h (shared header: nested class CMurderTwistLeader, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_MurderTwistScript.h"
CScriptBase* NScript::CV_MurderTwistScript::CMurderTwistLeader::GetParentScript() const
{
    return &ParentClass;
}
