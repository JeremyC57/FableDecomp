#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x004E2BC7: Route block freed, then the target pointer released, then ~CTCBase.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct CTCBase { virtual void v00() {} void* ParentThing; ~CTCBase() { Log('B'); } };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; ~CBaseIntelligentPointer() { Log('P'); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} };
struct SRouteNode { float x, y; unsigned char flags; };
struct SRouteVector {
    SRouteNode* _M_start; SRouteNode* _M_finish; SRouteNode* _M_end_of_storage;
    ~SRouteVector() { if (_M_start) { Log('F'); std::free(_M_start); } }
};
struct CTCPreCalculatedNavigationRoute : CTCBase {
    CIntelligentPointer PThingToCalculateRouteTo;
    unsigned long ThingToCalculateRouteToUID[2];
    SRouteVector Route;
    virtual void v00() {}
    ~CTCPreCalculatedNavigationRoute();
};
CTCPreCalculatedNavigationRoute::~CTCPreCalculatedNavigationRoute()
{
}
int main()
{
    { CTCPreCalculatedNavigationRoute r; r.Route._M_start = (SRouteNode*)std::malloc(sizeof(SRouteNode) * 2); }
    if (std::strcmp(g_log, "FPB") != 0) { std::printf("004e2bc7_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { CTCPreCalculatedNavigationRoute r; r.Route._M_start = 0; }
    if (std::strcmp(g_log, "PB") != 0) { std::printf("004e2bc7_TEST FAIL\n"); return 1; }
    std::printf("004e2bc7_TEST PASS\n");
    return 0;
}
