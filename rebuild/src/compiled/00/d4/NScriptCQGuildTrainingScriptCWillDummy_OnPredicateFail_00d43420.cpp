// NScript::CQ_GuildTrainingScript::CWillDummy::OnPredicateFail() -- empty override (the entity does nothing when its predicate fails)
// Leaf: ?OnPredicateFail@CWillDummy@CQ_GuildTrainingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D43420, 1 bytes: ret.
// Identity (naming wave 35, family ent): reg 00d3c3bf "WillDummy" in 00d3bc60; Alloc 00d43360; vtable 012ce364 slot 5; PDB ?OnPredicateFail@CWillDummy@CQ_GuildTrainingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d43420.
//   ?OnPredicateFail@CWillDummy@CQ_GuildTrainingScript@NScript@@EAEXXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: engine/CQ_GuildTrainingScript.h (shared header: nested class CWillDummy declares OnPredicateFail()); the Alloc body (if any) is not part of this graded body.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_GuildTrainingScript.h"
void NScript::CQ_GuildTrainingScript::CWillDummy::OnPredicateFail()
{
}
