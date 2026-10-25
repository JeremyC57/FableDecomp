// Behaviour test for AreAllThingsInVectorDead (0x00CBED00): a thing counts as dead when it is not alive or is
// unconscious; the result is "every thing counted".
#include <cstdio>

struct CScriptThing {
    bool alive;
    bool unconscious;
    bool IsAlive() const { return alive; }
    bool IsUnconscious() const { return unconscious; }
};
struct CThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    const CScriptThing* begin() const { return _M_start; }
    const CScriptThing* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};

bool AreAllThingsInVectorDead(const CThingVector& things)
{
    unsigned int dead = 0;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (!it->IsAlive() || it->IsUnconscious())
            ++dead;
    return dead == things.size();
}

int main()
{
    CScriptThing t[3];
    t[0].alive = false; t[0].unconscious = false;
    t[1].alive = true;  t[1].unconscious = true;
    t[2].alive = false; t[2].unconscious = true;
    CThingVector v;
    v._M_start = t; v._M_finish = t + 3;
    bool allDead = AreAllThingsInVectorDead(v);
    t[1].unconscious = false;
    bool oneAwake = AreAllThingsInVectorDead(v);
    v._M_finish = t;
    bool empty = AreAllThingsInVectorDead(v);
    if (!allDead || oneAwake || !empty) {
        std::printf("00cbed00_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbed00_TEST PASS\n");
    return 0;
}