#include <cstdio>
class CThing { public:
    unsigned char m_flags;
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CBaseIntelligentPointer {
    CThing* PData;
    CThing* GetPItem() const { return PData; }
};
inline bool IS_THING_DEAD(const CBaseIntelligentPointer& pointer)
{
    if (pointer.GetPItem() != 0) {
        if (!pointer.GetPItem()->IsDead())
            return false;
    }
    return true;
}
struct SRouteNode { float x, y; unsigned char flags; };
struct CRetailVector_RouteNode {
    SRouteNode* _Myfirst; SRouteNode* _Mylast; SRouteNode* _Myend;
    bool empty() const { return _Myfirst == _Mylast; }
};
class CTCPreCalculatedNavigationRoute { public:
    CBaseIntelligentPointer PThingToCalculateRouteTo;
    CRetailVector_RouteNode Route;
    long HasRoute() const
    {
        if (!IS_THING_DEAD(PThingToCalculateRouteTo) && !Route.empty())
            return 1;
        return 0;
    }
};
int main()
{
    SRouteNode nodes[2];
    CThing t; t.m_flags = 0;
    CTCPreCalculatedNavigationRoute r;
    r.PThingToCalculateRouteTo.PData = 0; r.Route._Myfirst = nodes; r.Route._Mylast = nodes + 2; r.Route._Myend = nodes + 2;
    int bad = 0;
    if (r.HasRoute() != 0) bad = 1;                     // no target
    r.PThingToCalculateRouteTo.PData = &t;
    if (r.HasRoute() != 1) bad = 1;                     // live target, route
    t.m_flags = 0x81;
    if (r.HasRoute() != 0) bad = 1;                     // dead target
    t.m_flags = 0x80; r.Route._Mylast = nodes;
    if (r.HasRoute() != 0) bad = 1;                     // empty route
    if (bad) { std::printf("00800500_TEST FAIL\n"); return 1; }
    std::printf("00800500_TEST PASS\n");
    return 0;
}