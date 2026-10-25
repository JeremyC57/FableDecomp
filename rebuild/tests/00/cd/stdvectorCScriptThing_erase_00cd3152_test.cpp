// Behaviour test for 0x00CD3152 vector<CScriptThing>::erase(position): the tail shifts down, the last element is destroyed, position is returned.
#include <cstdio>
static int g_destroyed = 0;
struct Thing {
    int id;
    virtual ~Thing() { ++g_destroyed; }
};
struct Vec {
    Thing* start; Thing* finish;
    Thing* erase(Thing* position)
    {
        if (position + 1 != finish)
            for (Thing* p = position + 1; p != finish; ++p) p[-1].id = p->id;   // the forward __copy (assignment)
        --finish;
        finish->~Thing();
        return position;
    }
};
int main()
{
    Thing* storage = new Thing[4];
    for (int i = 0; i < 4; ++i) storage[i].id = 10 + i;
    Vec v; v.start = storage; v.finish = storage + 4;
    Thing* r = v.erase(storage + 1);
    if (r != storage + 1 || v.finish != storage + 3 || storage[1].id != 12 || storage[2].id != 13 || g_destroyed != 1) {
        std::printf("00cd3152_TEST FAIL\n"); return 1;
    }
    r = v.erase(storage + 2);   // last element: no shift
    if (r != storage + 2 || v.finish != storage + 2 || storage[1].id != 12 || g_destroyed != 2) { std::printf("00cd3152_TEST FAIL\n"); return 1; }
    std::printf("00cd3152_TEST PASS\n");
    return 0;
}