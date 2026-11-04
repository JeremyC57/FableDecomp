// NScript::CQ_SummoningTheShipScript::CSummonerMinion::OnPredicateFail() -- empty override (the entity does nothing when its predicate fails)
// Leaf: ?OnPredicateFail@CSummonerMinion@CQ_SummoningTheShipScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF2A10, 1 bytes: ret.
// Identity (naming wave 35, family ent): reg 00df14b3 "SummonerMinion" in 00df13a0; Alloc 00df2960; vtable 012de2b8 slot 5; PDB ?OnPredicateFail@CSummonerMinion@CQ_SummoningTheShipScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00df2a10.
//   ?OnPredicateFail@CSummonerMinion@CQ_SummoningTheShipScript@NScript@@EAEXXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_SummoningTheShipScript.h (shared header: nested class CSummonerMinion declares OnPredicateFail()); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_SummoningTheShipScript.h"
void NScript::CQ_SummoningTheShipScript::CSummonerMinion::OnPredicateFail()
{
}
