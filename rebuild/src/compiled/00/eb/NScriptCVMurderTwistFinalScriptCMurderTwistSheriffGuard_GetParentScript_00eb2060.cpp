// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB2060, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00eaeacd "MurderTwistSheriffGuard" in 00eaea20; Alloc 00eb1f80; vtable 012efee8 slot 3; PDB ?GetParentScript@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00eb2060.
//   ?GetParentScript@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CV_MurderTwistFinalScript.h (shared header: nested class CMurderTwistSheriffGuard, ParentClass pointer); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_MurderTwistFinalScript.h"
CScriptBase* NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::GetParentScript() const
{
    return ParentClass;
}
