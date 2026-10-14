#include <cstdio>
static int g_copies = 0;
struct CScriptThing {
    long m_id; long m_a; long m_b;
    CScriptThing(long id) : m_id(id), m_a(0), m_b(0) { }
    CScriptThing(const CScriptThing& other) : m_id(other.m_id), m_a(other.m_a), m_b(other.m_b) { ++g_copies; }
};
namespace NScript {
struct CIsThingAlive {
    virtual void v00() { }
    CScriptThing Thing;
    CIsThingAlive(long id) : Thing(id) { }
    CIsThingAlive(const CIsThingAlive& other) : Thing(other.Thing) { }
};
template<class TLeft, class TRight>
struct CPredicate_And {
    virtual void v00() { }
    TLeft  m_left;
    TRight m_right;
    CPredicate_And(const TLeft& left, const TRight& right) : m_left(left), m_right(right) { }
    CPredicate_And(const CPredicate_And& other) : m_left(other.m_left), m_right(other.m_right) { }
};
}
using namespace NScript;
static long Sum(const CIsThingAlive& a) { return a.Thing.m_id; }
template<class L, class R> static long Sum(const CPredicate_And<L, R>& p) { return Sum(p.m_left) * 7 + Sum(p.m_right); }
int main()
{
    CIsThingAlive a1(1), a2(2), a3(3), a4(4), a5(5);
    CPredicate_And<CIsThingAlive, CIsThingAlive> p2(a1, a2);
    CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive> p3(p2, a3);
    CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive> p4(p3, a4);
    CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive> p5(p4, a5);
    long before = Sum(p5);
    g_copies = 0;
    CPredicate_And<CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive, CIsThingAlive>, CIsThingAlive>, CIsThingAlive>, CIsThingAlive> copy(p5);
    if (Sum(copy) != before || g_copies != 5 || sizeof(copy) != sizeof(p5)) { std::printf("00ef7060_TEST FAIL\n"); return 1; }
    std::printf("00ef7060_TEST PASS\n");
    return 0;
}
