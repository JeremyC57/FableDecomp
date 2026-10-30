#ifndef FABLE_ENGINE_CPREDICATE_AND_H
#define FABLE_ENGINE_CPREDICATE_AND_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): template <class P1, class P2> NScript::CPredicate_And
// : CPredicateBase {P1 Predicate1 +0x04; P2 Predicate2} (Ego_r.pdb CPredicate_And<CIsThingAlive,CIsThingAlive> 0x24). For
// <CIsThingAlive, CIsThingAlive>: vtable 0x012E1B70 (no RTTI): 0 operator() 0x00E23F00, 1 Clone 0x00E247A0, 2 scalar deleting
// dtor 0x00E245A0; ctor (const P1&, const P2&) 0x00E24760 (`ret 8`). Names (Ego_r.pdb + FableWin.pdb):
// ??R?$CPredicate_And@..@NScript@@UBE_NXZ, ?Clone@?$CPredicate_And@..@@UBEPAVCPredicateBase@2@XZ, ??0..QAE@ABVCIsThingAlive@1@0@Z,
// ??1..UAE@XZ. NScript::operator&& (both PDBs: ??$?V..@NScript@@YI?AV?$CPredicate_And@..@0@AB..0@Z, __fastcall) builds the
// pair by value; retail expands it inline (landed 0x00EF54A0), so it is defined inline here.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CPredicate_And.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CPredicateBase.h"

namespace NScript {
template <class P1, class P2> class CPredicate_And : public CPredicateBase {
public:
    CPredicate_And(const P1&, const P2&);                                // <CIsThingAlive,CIsThingAlive> 0x00E24760
    virtual bool operator()() const;                                     // slot 0
    virtual CPredicateBase* Clone() const;                               // slot 1
    virtual ~CPredicate_And();                                           // slot 2
    P1 Predicate1;                                                       // +0x04
    P2 Predicate2;
};
template <class P1, class P2> inline CPredicate_And<P1, P2> FABLE_FASTCALL operator&&(const P1& p1, const P2& p2)
{
    return CPredicate_And<P1, P2>(p1, p2);
}
}

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CPREDICATE_AND_H
