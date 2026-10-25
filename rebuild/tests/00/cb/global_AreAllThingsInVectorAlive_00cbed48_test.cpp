// Behaviour test for AreAllThingsInVectorAlive (0x00CBED48): true when every thing in the vector is alive.
#include <cstdio>

struct CScriptThing {
    bool alive;
    bool IsAlive() const { return alive; }
};
struct CThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    const CScriptThing* begin() const { return _M_start; }
    const CScriptThing* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};

bool AreAllThingsInVectorAlive(const CThingVector& things)
{
    unsigned int alive = 0;
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (it->IsAlive())
            ++alive;
    return alive == things.size();
}

int main()
{
    CScriptThing t[3];
    t[0].alive = true; t[1].alive = true; t[2].alive = true;
    CThingVector v;
    v._M_start = t; v._M_finish = t + 3;
    bool all = AreAllThingsInVectorAlive(v);
    t[2].alive = false;
    bool notAll = AreAllThingsInVectorAlive(v);
    v._M_finish = t;
    bool empty = AreAllThingsInVectorAlive(v);
    if (!all || notAll || !empty) {
        std::printf("00cbed48_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbed48_TEST PASS\n");
    return 0;
}