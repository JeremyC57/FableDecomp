// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEC10, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): Anniversary wave 2026-11-04 (high): vtable 0x012C3208 slot 3; Anniversary RTTI NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost (string-literal match, validated 257/258); PDB name set has the GetParentScript member of NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost; slot layout from named TLC siblings; function referenced by this vtable only.
//   ?GetParentScript@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CPersonalScript_GlobalThingsScript.h (shared header: nested class CArcheryCompetitionSignpost, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CPersonalScript_GlobalThingsScript.h"
CScriptBase* NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::GetParentScript() const
{
    return &ParentClass;
}
