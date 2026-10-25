// behaviour test for 0x00ef6d90 NScript::CPredicate_And (four terms)::~CPredicate_And: fourth term, then the nested three- and two-term Ands
#include <cstdio>

static int g_order[16];
static int g_count = 0;

class CScriptThing {                                     // stand-in: an id and an alive flag
public:
    int  Id;
    bool Alive;
    CScriptThing(int id, bool alive) : Id(id), Alive(alive) { }
    CScriptThing(const CScriptThing& other) : Id(other.Id), Alive(other.Alive) { }
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
    virtual ~CIsThingAlive() { if (g_count < 16) g_order[g_count] = Thing.Id; ++g_count; }   // stand-in for 0x00E241B0
    virtual bool operator()() const { return Thing.Alive; }
    virtual CPredicateBase* Clone() const { return new CIsThingAlive(*this); }
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

using namespace NScript;
typedef CPredicate_And<CIsThingAlive, CIsThingAlive> TAnd2;
typedef CPredicate_And<TAnd2, CIsThingAlive> TAnd3;
typedef CPredicate_And<TAnd3, CIsThingAlive> TAnd4;
typedef CPredicate_And<TAnd4, CIsThingAlive> TAnd5;

static CIsThingAlive Leaf(int id, bool alive) { return CIsThingAlive(CScriptThing(id, alive)); }

static TAnd4 Build(int deadId)
{
    return TAnd4(TAnd3(TAnd2(Leaf(1, deadId != 1), Leaf(2, deadId != 2)), Leaf(3, deadId != 3)), Leaf(4, deadId != 4));
}

int main()
{
    const int n = 4;
    // destruction order: the outermost right term first, the innermost left term last (ids n .. 1)
    {
        TAnd4 p = Build(0);
        g_count = 0;
    }
    if (g_count != n) { std::printf("00ef6d90_TEST FAIL\n"); return 1; }
    for (int i = 0; i < n; ++i)
        if (g_order[i] != n - i) { std::printf("00ef6d90_TEST FAIL\n"); return 1; }
    // the virtual destructor through the base pointer runs the same chain
    {
        TAnd4 p = Build(0);
        CPredicateBase* clone = p.Clone();
        g_count = 0;
        delete clone;
        if (g_count != n || g_order[0] != n || g_order[n - 1] != 1) { std::printf("00ef6d90_TEST FAIL\n"); return 1; }
        g_count = 0;
    }
    // the predicate itself: true only when every term is alive
    for (int dead = 0; dead <= n; ++dead) {
        TAnd4 p = Build(dead);
        bool expect = (dead == 0);
        if (p() != expect) { std::printf("00ef6d90_TEST FAIL\n"); return 1; }
    }
    std::printf("00ef6d90_TEST PASS\n");
    return 0;
}