// Behaviour test for 0x00CDF120 NScript::CIsThingAlive::operator()(): the predicate forwards to its member thing's virtual IsAlive.
#include <cstdio>
class CScriptThing {
public:
    CScriptThing(bool alive) : Alive(alive) { }
    virtual ~CScriptThing() { }
    virtual bool IsAlive() const { return Alive; }
    bool Alive;
};
class CDeadThing : public CScriptThing {                 // a derived implementation: the call must stay virtual
public:
    CDeadThing() : CScriptThing(true) { }
    virtual bool IsAlive() const { return false; }
};
class CPredicateBase {
public:
    virtual bool operator()() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase {
public:
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const;
    const CScriptThing& Thing;
};
bool CIsThingAlive::operator()() const
{
    return Thing.IsAlive();
}
int main()
{
    CScriptThing alive(true), dead(false);
    CDeadThing derived;
    CIsThingAlive a(alive), d(dead), v(derived);
    const CPredicateBase& pa = a;
    if (!pa() || d() || v()) { std::printf("00cdf120_TEST FAIL\n"); return 1; }
    alive.Alive = false;
    if (pa()) { std::printf("00cdf120_TEST FAIL\n"); return 1; }
    std::printf("00cdf120_TEST PASS\n");
    return 0;
}