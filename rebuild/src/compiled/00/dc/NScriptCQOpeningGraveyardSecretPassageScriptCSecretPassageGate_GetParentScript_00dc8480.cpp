// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGate::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CSecretPassageGate@CQ_OpeningGraveyardSecretPassageScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC8480, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00dc29a8 "SecretPassageGate" in 00dc27c0; Alloc 00dc83e0; vtable 012da9e4 slot 3; PDB ?GetParentScript@CSecretPassageGate@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00dc8480.
//   ?GetParentScript@CSecretPassageGate@CQ_OpeningGraveyardSecretPassageScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_OpeningGraveyardSecretPassageScript.h (shared header: nested class CSecretPassageGate, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
CScriptBase* NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGate::GetParentScript() const
{
    return &ParentClass;
}
