// NScript::CPredicate_And::operator()() const -- (CPredicate_And<CIsThingConscious, CIsThingConscious >) both predicates hold: Predicate1() && Predicate2(), short-circuit
// Leaf: ??R?$CPredicate_And@VCIsThingConscious@NScript@@V12@@NScript@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF86D0, 38 bytes (+ int3 padding), thiscall; slot 0 of the RTTI-less vtable
//   0x012F687C {operator(), Clone 0x00EF8780, deleting destructor}.
// Identity: NScript::CPredicate_And<CIsThingConscious, CIsThingConscious >::operator()() const -- proven; evidence: the Clone in slot 1 allocates 0x24 bytes and rebuilds
//   CIsThingConscious vtables 0x012C2FE8 at +0x04 and +0x14; the leaf vtables are told apart by their own operator() (0x012C32F0: one CScriptThing slot-75 test = CIsThingAlive;
//   0x012C2FE8: the same test plus a negated slot-61 test = CIsThingConscious; 0x012EA43C: `!*Variable` = CIsVariableFalse), and
//   only that assignment makes all four 16+16-byte instantiations members of the FableWin.pdb name set, which holds
//   ??R?$CPredicate_And@VCIsThingConscious@NScript@@V12@@NScript@@UBE_NXZ. Family G051 (8 CPredicate_And instantiations; the propagated five-deep label is right only for 0x00EF6D20).
// Callees: the two sub-predicates' virtual operator() (slot 0) through their own vtables.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase vtable slots operator() / Clone / ~; CIsThingAlive, CIsThingConscious: : CPredicateBase, CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
#include "engine/CIsThingConscious.h"
namespace NScript {
class CIsVariableFalse : public CPredicateBase {     // 8 bytes, vtable 0x012EA43C
public:
    CIsVariableFalse(bool* variable);
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    bool* Variable;                                  // +0x04
};
template <class P1, class P2> class CPredicate_And : public CPredicateBase {
public:
    CPredicate_And(const P1& p1, const P2& p2) : Predicate1(p1), Predicate2(p2) { }
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    P1 Predicate1;                                   // +0x04
    P2 Predicate2;
};
template <class P1, class P2> bool CPredicate_And<P1, P2>::operator()() const
{
    return Predicate1() && Predicate2();
}
template bool CPredicate_And<CIsThingConscious, CIsThingConscious >::operator()() const;
}