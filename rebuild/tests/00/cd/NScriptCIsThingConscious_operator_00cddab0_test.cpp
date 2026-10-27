#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
class CScriptThing {
public:
    bool Alive, Unconscious; int Id;
    CScriptThing(int id) : Alive(true), Unconscious(false), Id(id) { }
    virtual ~CScriptThing() { }
    virtual bool IsAlive() const { return Alive; }
    virtual bool IsUnconscious() const { return Unconscious; }
};
class CPredicateBase {
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingConscious : public CPredicateBase {
public:
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const { return Thing.IsAlive() && !Thing.IsUnconscious(); }
    virtual CPredicateBase* Clone() const { return new CIsThingConscious(*this); }
    CScriptThing Thing;
};
int main()
{
    int ok = 1;
    CScriptThing t(3); CIsThingConscious p(t);
    ok = p();
    p.Thing.Unconscious = true; ok = ok && !p();
    p.Thing.Unconscious = false; p.Thing.Alive = false; ok = ok && !p();
    if (!ok) { std::printf("00cddab0_TEST FAIL\n"); return 1; }
    std::printf("00cddab0_TEST PASS\n");
    return 0;
}