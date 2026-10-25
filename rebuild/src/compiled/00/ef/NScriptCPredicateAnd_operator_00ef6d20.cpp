// NScript::CPredicate_And::operator()() const -- (CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >) both predicates hold: Predicate1() && Predicate2(), short-circuit
// Leaf: ??R?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF6D20, 38 bytes (+ int3 padding), thiscall; slot 0 of the RTTI-less vtable
//   0x012F6798 {operator(), Clone 0x00EF7160, deleting destructor}.
// Identity: NScript::CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >::operator()() const -- proven; evidence: the Clone in slot 1 allocates 0x74 bytes and rebuilds
//   the 0x00EF6CF0 vtable 0x012F678C at +0x04, 0x012F6780 at +0x08 and CIsThingAlive at +0x44 / +0x54 / +0x64; the leaf vtables are told apart by their own operator() (0x012C32F0: one CScriptThing slot-75 test = CIsThingAlive;
//   0x012C2FE8: the same test plus a negated slot-61 test = CIsThingConscious; 0x012EA43C: `!*Variable` = CIsVariableFalse), and
//   only that assignment makes all four 16+16-byte instantiations members of the FableWin.pdb name set, which holds
//   ??R?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@V?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@VCIsThingAlive@2@@NScript@@UBE_NXZ. Family G051 (8 CPredicate_And instantiations; the propagated five-deep label is right only for 0x00EF6D20).
// Callees: the two sub-predicates' virtual operator() (slot 0) through their own vtables.
class CScriptThing {                                 // retail 12 bytes
public:
    virtual ~CScriptThing();
    unsigned char _data[8];
};
namespace NScript {
class CPredicateBase {
public:
    virtual bool operator()() const = 0;             // slot 0
    virtual CPredicateBase* Clone() const = 0;       // slot 1
    virtual ~CPredicateBase() { }                    // slot 2
};
class CIsThingAlive : public CPredicateBase {        // 0x10 bytes, vtable 0x012C32F0
public:
    CIsThingAlive(const CScriptThing& thing);
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    CScriptThing Thing;                              // +0x04
};
class CIsThingConscious : public CPredicateBase {    // 0x10 bytes, vtable 0x012C2FE8
public:
    CIsThingConscious(const CScriptThing& thing);
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    CScriptThing Thing;                              // +0x04
};
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
template bool CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >, CIsThingAlive >::operator()() const;
}