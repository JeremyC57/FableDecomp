// NScript::CQ_SummoningTheShipScript::CSummonerAttacker::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CSummonerAttacker@CQ_SummoningTheShipScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF2290, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00df144f "SummonerAttacker" in 00df13a0; Alloc 00df21f0; vtable 012de274 slot 3; PDB ?GetParentScript@CSummonerAttacker@CQ_SummoningTheShipScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00df2290.
//   ?GetParentScript@CSummonerAttacker@CQ_SummoningTheShipScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_SummoningTheShipScript.h (shared header: nested class CSummonerAttacker, ParentClass reference); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_SummoningTheShipScript.h"
CScriptBase* NScript::CQ_SummoningTheShipScript::CSummonerAttacker::GetParentScript() const
{
    return &ParentClass;
}
