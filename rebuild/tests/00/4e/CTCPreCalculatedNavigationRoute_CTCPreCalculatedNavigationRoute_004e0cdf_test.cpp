#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing { int id; };
static int g_globalAdds = 0;
struct CTCBase {
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
    void AddToGlobalUpdateTCs() { if (!IsInGlobalUpdateTCs) { IsInGlobalUpdateTCs = true; ++g_globalAdds; } }
};
struct CIntelligentPointer {
    void* PData;
    CIntelligentPointer() : PData(0) { }
    virtual ~CIntelligentPointer() { }
};
struct SRouteNode;
struct CRetailVector_RouteNode {
    SRouteNode* _Myfirst; SRouteNode* _Mylast; SRouteNode* _Myend;
    CRetailVector_RouteNode() : _Myfirst(0), _Mylast(0), _Myend(0) { }
};
struct CTCPreCalculatedNavigationRoute : CTCBase {
    CIntelligentPointer     PThingToCalculateRouteTo;
    u64                     ThingToCalculateRouteToUID;
    CRetailVector_RouteNode Route;
    CTCPreCalculatedNavigationRoute(CThing* thing) : CTCBase(thing), PThingToCalculateRouteTo(), ThingToCalculateRouteToUID(0), Route() { }
};
int main()
{
    CThing t; t.id = 3;
    CTCPreCalculatedNavigationRoute r(&t);
    if (r.ParentThing != &t || r.PThingToCalculateRouteTo.PData != 0 || r.ThingToCalculateRouteToUID != 0 ||
        r.Route._Myfirst != 0 || r.Route._Mylast != 0 || r.Route._Myend != 0) { std::printf("004e0cdf_TEST FAIL\n"); return 1; }
    std::printf("004e0cdf_TEST PASS\n");
    return 0;
}