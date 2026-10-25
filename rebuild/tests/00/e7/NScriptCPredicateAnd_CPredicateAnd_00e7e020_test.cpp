// Behaviour test for 0x00E7E020 NScript::CPredicate_And<CIsThingAlive, CIsVariableFalse>::CPredicate_And:
// copy both predicates into the pair (the thing handle copied once); the result is their logical and
#include <stdio.h>
static int gThingCopies = 0;
class CScriptThing {
public:
    long  m_id;
    void* m_pCounted;
    CScriptThing(long id) : m_id(id), m_pCounted(0) { }
    CScriptThing(const CScriptThing& other) : m_id(other.m_id), m_pCounted(other.m_pCounted) { ++gThingCopies; }
    virtual ~CScriptThing() { }
};
namespace NScript {
class CPredicateBase {
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase {
public:
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual ~CIsThingAlive() { }
    virtual bool operator()() const { return Thing.m_id > 0; }          // stand-in: positive ids are alive
    virtual CPredicateBase* Clone() const { return new CIsThingAlive(*this); }
};
class CIsVariableFalse : public CPredicateBase {
public:
    const bool* Variable;
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual ~CIsVariableFalse() { }
    virtual bool operator()() const { return !*Variable; }
    virtual CPredicateBase* Clone() const { return new CIsVariableFalse(*this); }
};
template <class P1, class P2> class CPredicate_And : public CPredicateBase {
public:
    P1 Predicate1;
    P2 Predicate2;
    CPredicate_And(const P1& predicate1, const P2& predicate2) : Predicate1(predicate1), Predicate2(predicate2) { }
    virtual ~CPredicate_And() { }
    virtual bool operator()() const { return Predicate1() && Predicate2(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};
}
typedef NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsVariableFalse> CAliveAndUnset;
int main()
{
    CScriptThing thing(5);
    NScript::CIsThingAlive alive(thing);
    bool flag = false;
    NScript::CIsVariableFalse unset(flag);
    int before = gThingCopies;
    CAliveAndUnset both(alive, unset);
    bool ok = true;
    ok = ok && gThingCopies - before == 1;
    ok = ok && both.Predicate1.Thing.m_id == 5 && both.Predicate2.Variable == &flag;
    const NScript::CPredicateBase& base = both;
    ok = ok && base();                          // alive and the flag still false
    flag = true;
    ok = ok && !base();                         // the variable is read through the stored pointer
    flag = false;
    alive.Thing.m_id = -1;                      // the original changing does not reach the copy
    ok = ok && base();
    NScript::CPredicateBase* clone = both.Clone();
    ok = ok && (*clone)();
    delete clone;
    if (ok) { printf("00e7e020_TEST PASS\n"); return 0; }
    printf("00e7e020_TEST FAIL\n");
    return 1;
}