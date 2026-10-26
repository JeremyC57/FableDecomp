// Behaviour test for 008006d0: final.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext* context) -- when the target thing exists and is alive, recompute the owner->target route and save its step count, then each step's local position and nav layer; otherwise save a step count of 0
// Retail: Fable.exe @ 0x008006D0, 480 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0, CalculateRouteFromTo 0x00800540,
//   CPersistContext::Transfer<long> 0x00410BE0 / <C2DVector> 0x00800B90 / <unsigned char> 0x00405A60, CTCBase::GetWorldMap
//   0x00686D20, CWorldMap::ConvertWorldMapCoordsToLocalCoords 0x004FC330, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, CCharString::ToConstChar 0x0099E4C0, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Layout: Ego_r.pdb CTCPreCalculatedNavigationRoute: PThingToCalculateRouteTo +0x0c, Route +0x20 (12-byte STLport vector of
//   9-byte pair<C2DVector, unsigned char> steps -- retail divides by 9); CThing region index short +0x8e, Dead bit +0x91.
#include <stdlib.h>
class C2DVector { public: float X; float Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_data;
    ~CCharString();                                 // 0x0099EAE0
    const char* ToConstChar() const;                // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830
CCharString __fastcall operator+(const char* a, const CCharString& b);       // 0x0099F690
class CThing { public:
    unsigned char  _pad00[0x8e];
    short          RegionIndex;                     // +0x8e
    unsigned char  _pad90;
    unsigned char  Dead : 1;                        // +0x91 bit 0
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CBaseIntelligentPointer {
    unsigned char _pad[0xc];
    CThing* GetPItem() const;                       // 0x00A01B50
};
class CWorldMap { public:
    C2DVector ConvertWorldMapCoordsToLocalCoords(long region, const C2DVector& pos);   // 0x004FC330
};
class CPersistContext { public:
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, C2DVector& value, const C2DVector& defaultValue);                // 0x00800B90
    void Transfer(const char* name, unsigned char& value, const unsigned char& defaultValue);        // 0x00405A60
};
#pragma pack(push, 1)
struct CNavStep { C2DVector Pos; unsigned char Layer; };
#pragma pack(pop)
struct CNavStepVector {
    CNavStep* _M_start;
    CNavStep* _M_finish;
    CNavStep* _M_end_of_storage;
    CNavStepVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CNavStepVector() { if (_M_start) free(_M_start); }
    long size() const { return _M_finish - _M_start; }
};
class CTCBase { public:
    void*   vtable;
    CThing* Owner;                                  // +0x04
    unsigned char _pad08[4];
    CWorldMap* GetWorldMap();                       // 0x00686D20
};
class CTCPreCalculatedNavigationRoute : public CTCBase { public:
    CBaseIntelligentPointer PThingToCalculateRouteTo;   // +0x0c
    unsigned __int64        ThingToCalculateRouteToUID; // +0x18
    CNavStepVector          Route;                      // +0x20
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CNavStepVector& route);   // 0x00800540
    void SaveRoute(CPersistContext* context);
};

template <class T>
__forceinline void SaveValue(CPersistContext* context, const char* name, T& value, const T& defaultValue)
{
    context->Transfer(name, value, defaultValue);
}

void CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext* context)
{
    if (PThingToCalculateRouteTo.GetPItem() && !PThingToCalculateRouteTo.GetPItem()->Dead)
    {
        CThing* owner = Owner;
        long region = owner->RegionIndex;
        CNavStepVector route;
        CalculateRouteFromTo(owner->GetPosition(), PThingToCalculateRouteTo.GetPItem()->GetPosition(), route);
        long steps = route.size();
        context->Transfer("NumberOfStepsOnRoute", steps, 0);
        long i = 0;
        if (i < steps)
        {
        C2DVector defaultPos(0.0f, 0.0f);
        unsigned char defaultLayer = 0;
        do
        {
            C2DVector pos = GetWorldMap()->ConvertWorldMapCoordsToLocalCoords(region, route._M_start[i].Pos);
            SaveValue(context, ("NavPosition" + GFIntToCharString(i)).ToConstChar(), pos, defaultPos);
            unsigned char layer = route._M_start[i].Layer;
            SaveValue(context, ("NavLayer" + GFIntToCharString(i)).ToConstChar(), layer, defaultLayer);
        } while (++i < steps);
        }
    }
    else
    {
        long steps = 0;
        context->Transfer("NumberOfStepsOnRoute", steps, 0);
    }
}

// ---- stubs and driver ----
static int g_getPItem = 0, g_getPos = 0, g_calc = 0, g_tLong = 0, g_tVec = 0, g_tByte = 0, g_getMap = 0, g_convert = 0;
static int g_intToStr = 0, g_plus = 0, g_toConst = 0, g_dtor = 0;
static CThing* g_target = 0;
static C3DVector g_ownerPos, g_targetPos;
static long g_routeSteps = 0;
static char g_names[16][64]; static int g_nameCount = 0;
static long g_lastLong = -1, g_lastLongDef = -1;
static C2DVector g_lastVec(0.0f, 0.0f), g_lastVecDef(1.0f, 1.0f);
static unsigned char g_lastByte = 0xff, g_lastByteDef = 0xff;
static long g_lastRegion = -1;
static const CThing* g_lastFrom = 0; static const C3DVector* g_calcFrom = 0; static const C3DVector* g_calcTo = 0;
static char g_pool[8][64]; static int g_poolNext = 0;
static CWorldMap g_worldMap;
static void record(const char* name) { strncpy(g_names[g_nameCount], name, 63); g_names[g_nameCount][63] = 0; ++g_nameCount; }
CThing* CBaseIntelligentPointer::GetPItem() const { ++g_getPItem; return g_target; }
const C3DVector& CThing::GetPosition() const { ++g_getPos; return (this == g_target) ? g_targetPos : g_ownerPos; }
CWorldMap* CTCBase::GetWorldMap() { ++g_getMap; return &g_worldMap; }
C2DVector CWorldMap::ConvertWorldMapCoordsToLocalCoords(long region, const C2DVector& pos)
{ ++g_convert; g_lastRegion = region; return C2DVector(pos.X + 100.0f, pos.Y + 200.0f); }
void CTCPreCalculatedNavigationRoute::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CNavStepVector& route)
{
    ++g_calc; g_calcFrom = &from; g_calcTo = &to;
    CHECK(route._M_start == 0 && route._M_finish == 0 && route._M_end_of_storage == 0);
    if (g_routeSteps > 0) {
        CNavStep* steps = (CNavStep*)malloc(sizeof(CNavStep) * g_routeSteps);
        for (long i = 0; i < g_routeSteps; ++i) { steps[i].Pos.X = (float)i; steps[i].Pos.Y = (float)(i * 10); steps[i].Layer = (unsigned char)(i + 1); }
        route._M_start = steps; route._M_finish = steps + g_routeSteps; route._M_end_of_storage = route._M_finish;
    }
}
void CPersistContext::Transfer(const char* name, long& value, const long& defaultValue) { ++g_tLong; record(name); g_lastLong = value; g_lastLongDef = defaultValue; }
void CPersistContext::Transfer(const char* name, C2DVector& value, const C2DVector& defaultValue) { ++g_tVec; record(name); g_lastVec = value; g_lastVecDef = defaultValue; }
void CPersistContext::Transfer(const char* name, unsigned char& value, const unsigned char& defaultValue) { ++g_tByte; record(name); g_lastByte = value; g_lastByteDef = defaultValue; }
CCharString __fastcall GFIntToCharString(long value)
{ ++g_intToStr; CCharString s; char* buf = g_pool[g_poolNext++ & 7]; sprintf(buf, "%ld", value); s.m_data = buf; return s; }
CCharString __fastcall operator+(const char* a, const CCharString& b)
{ ++g_plus; CCharString s; char* buf = g_pool[g_poolNext++ & 7]; sprintf(buf, "%s%s", a, (const char*)b.m_data); s.m_data = buf; return s; }
const char* CCharString::ToConstChar() const { ++g_toConst; return (const char*)m_data; }
CCharString::~CCharString() { ++g_dtor; }
static void reset() { g_getPItem = g_getPos = g_calc = g_tLong = g_tVec = g_tByte = g_getMap = g_convert = 0; g_intToStr = g_plus = g_toConst = g_dtor = 0; g_nameCount = 0; g_lastLong = g_lastLongDef = -1; g_lastRegion = -1; }
static unsigned char g_ownerBuf[0x100], g_targetBuf[0x100];
int main()
{
    CPersistContext ctx;
    memset(g_ownerBuf, 0, sizeof(g_ownerBuf)); memset(g_targetBuf, 0, sizeof(g_targetBuf));
    CThing* owner = (CThing*)g_ownerBuf; CThing* target = (CThing*)g_targetBuf;
    owner->RegionIndex = 7;
    CTCPreCalculatedNavigationRoute tc; memset(&tc, 0, sizeof(tc)); tc.Owner = owner;
    g_ownerPos.X = 1.0f; g_ownerPos.Y = 2.0f; g_ownerPos.Z = 3.0f; g_targetPos.X = 4.0f; g_targetPos.Y = 5.0f; g_targetPos.Z = 6.0f;
    // 1. no target thing: a step count of 0 is saved, nothing else
    reset(); g_target = 0;
    tc.SaveRoute(&ctx);
    CHECK(g_getPItem == 1 && g_calc == 0 && g_tLong == 1 && g_lastLong == 0 && g_lastLongDef == 0 && strcmp(g_names[0], "NumberOfStepsOnRoute") == 0);
    // 2. dead target: the same
    reset(); g_target = target; target->Dead = 1;
    tc.SaveRoute(&ctx);
    CHECK(g_getPItem == 2 && g_calc == 0 && g_tLong == 1 && g_lastLong == 0);
    // 3. live target, empty route
    reset(); target->Dead = 0; g_routeSteps = 0;
    tc.SaveRoute(&ctx);
    CHECK(g_getPItem == 3 && g_calc == 1 && g_calcFrom == &g_ownerPos && g_calcTo == &g_targetPos && g_getPos == 2);
    CHECK(g_tLong == 1 && g_lastLong == 0 && g_lastLongDef == 0 && g_tVec == 0 && g_tByte == 0 && g_nameCount == 1);
    // 4. live target, two steps: count, then each step's local position and layer with indexed names
    reset(); g_routeSteps = 2;
    tc.SaveRoute(&ctx);
    CHECK(g_tLong == 1 && g_lastLong == 2 && g_tVec == 2 && g_tByte == 2 && g_nameCount == 5);
    CHECK(strcmp(g_names[0], "NumberOfStepsOnRoute") == 0 && strcmp(g_names[1], "NavPosition0") == 0 && strcmp(g_names[2], "NavLayer0") == 0);
    CHECK(strcmp(g_names[3], "NavPosition1") == 0 && strcmp(g_names[4], "NavLayer1") == 0);
    CHECK(g_getMap == 2 && g_convert == 2 && g_lastRegion == 7);
    CHECK(g_lastVec.X == 101.0f && g_lastVec.Y == 210.0f && g_lastVecDef.X == 0.0f && g_lastVecDef.Y == 0.0f);
    CHECK(g_lastByte == 2 && g_lastByteDef == 0);
    CHECK(g_intToStr == 4 && g_plus == 4 && g_toConst == 4 && g_dtor >= 8);   // the stubs' by-value returns add copies at /Od
    if (fails == 0) printf("008006d0_TEST PASS\n"); else printf("008006d0_TEST FAIL\n");
    return fails;
}