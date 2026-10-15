#include <cstdio>
#include <cstring>
#include <math.h>

struct CThing { int id; };
static int g_temps = 0, g_tempDtors = 0;
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer() : m_pItem(0) { }
    CIntelligentPointer(CThing* t) : m_pItem(t) { ++g_temps; }
    ~CIntelligentPointer() { ++g_tempDtors; }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { m_pItem = o.m_pItem; return *this; }
};
struct CTCAssassinRush {
    bool StartTrailDelay; CIntelligentPointer CollidedThing;
    void SetThingCollisionPtr(CThing* thing) { if (!StartTrailDelay) CollidedThing = thing; }
};
int main()
{
    CThing a, b; a.id = 1; b.id = 2;
    {
        CTCAssassinRush r; r.StartTrailDelay = false;
        r.SetThingCollisionPtr(&a);
        if (r.CollidedThing.m_pItem != &a || g_temps != 1 || g_tempDtors != 1) { std::printf("004cbfb0_TEST FAIL\n"); return 1; }
        r.StartTrailDelay = true; r.SetThingCollisionPtr(&b);
        if (r.CollidedThing.m_pItem != &a || g_temps != 1) { std::printf("004cbfb0_TEST FAIL\n"); return 1; }
    }
    std::printf("004cbfb0_TEST PASS\n");
    return 0;
}
