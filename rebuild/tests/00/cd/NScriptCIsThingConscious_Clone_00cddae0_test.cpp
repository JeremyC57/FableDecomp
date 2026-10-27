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
    CScriptThing t(3); t.Unconscious = true; CIsThingConscious p(t);
    CPredicateBase* c = p.Clone();
    CIsThingConscious* cc = (CIsThingConscious*)c;
    ok = c != 0 && c != &p && cc->Thing.Id == 3 && cc->Thing.Unconscious && !(*c)();
    delete c;
    if (!ok) { std::printf("00cddae0_TEST FAIL\n"); return 1; }
    std::printf("00cddae0_TEST PASS\n");
    return 0;
}