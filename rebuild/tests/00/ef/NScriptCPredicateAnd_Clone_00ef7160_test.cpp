#include <cstdio>
#include <cstring>
#include <cmath>
static int g_thingCopies = 0;
class CScriptThing {
public:
    int Id; bool Alive;
    CScriptThing(int id, bool alive) : Id(id), Alive(alive) { }
    CScriptThing(const CScriptThing& o) : Id(o.Id), Alive(o.Alive) { ++g_thingCopies; }
};
class CPredicateBase {
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase {
public:
    CScriptThing Thing;
    CIsThingAlive(const CScriptThing& t) : Thing(t) { }
    CIsThingAlive(const CIsThingAlive& o) : CPredicateBase(), Thing(o.Thing) { }
    virtual bool operator()() const { return Thing.Alive; }
    virtual CPredicateBase* Clone() const { return new CIsThingAlive(*this); }
};
template <class L, class R> class CPredicate_And : public CPredicateBase {
public:
    L Left; R Right;
    CPredicate_And(const L& l, const R& r) : Left(l), Right(r) { }
    CPredicate_And(const CPredicate_And& o) : CPredicateBase(), Left(o.Left), Right(o.Right) { }
    virtual bool operator()() const { return Left() && Right(); }
    virtual CPredicateBase* Clone() const { return new CPredicate_And(*this); }
};
typedef CPredicate_And<CIsThingAlive, CIsThingAlive> P1;
typedef CPredicate_And<P1, CIsThingAlive> P2;
typedef CPredicate_And<P2, CIsThingAlive> P3;
typedef CPredicate_And<P3, CIsThingAlive> P4;
typedef CPredicate_And<P4, CIsThingAlive> P5;

int main()
{
    CScriptThing t(1, true);
    CIsThingAlive a[6] = { CIsThingAlive(t), CIsThingAlive(t), CIsThingAlive(t), CIsThingAlive(t), CIsThingAlive(t), CIsThingAlive(t) };
    P5 p(P4(P3(P2(P1(a[0], a[1]), a[2]), a[3]), a[4]), a[5]);
    g_thingCopies = 0;
    CPredicateBase* c = p.Clone();
    if (g_thingCopies != 6) do { std::printf("00ef7160_TEST FAIL\n"); return 1; } while (0);                      // every term's CScriptThing is copied once
    if (!(*c)()) do { std::printf("00ef7160_TEST FAIL\n"); return 1; } while (0);
    p.Right.Thing.Alive = false;                          // the clone is a deep copy
    if (!(*c)() || p()) do { std::printf("00ef7160_TEST FAIL\n"); return 1; } while (0);
    P5* cc = static_cast<P5*>(c);
    cc->Right.Thing.Alive = false;
    if ((*c)()) do { std::printf("00ef7160_TEST FAIL\n"); return 1; } while (0);
    delete c;
    std::printf("00ef7160_TEST PASS\n");
    return 0;
}