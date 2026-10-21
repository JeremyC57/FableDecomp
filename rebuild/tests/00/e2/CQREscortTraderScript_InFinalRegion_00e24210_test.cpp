// Behaviour test for 00e24210: src/00e24210.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CQR_EscortTraderScript::InFinalRegion()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQR_EscortTraderScript::InFinalRegion() -- is the currently loaded region the last region of the trade route's static route list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24210, length 68 bytes.
// Callees: Compare @ 0x00411570 (`__fastcall(a, b)`, landed) under the inlined CCharString operator== (same
// block, null blocks, lengths at +4, then Compare == 0 as `neg ; sbb ; inc`).
// Layout (Ego_r.pdb CQR_EscortTraderScript - 0x14, then - 0x18 after the six PDB 16-byte vectors): TradeRoute
// +0xc4, RouteDynamic[3] +0xc8, RouteStatic[3] +0xec (12-byte vectors {first, last, end}),
// CurrentLoadedRegion +0x114.
// Shape: `return CurrentLoadedRegion == RouteStatic[TradeRoute].back();` with back() = `*(m_pLast - 1)`. The inline
// CCharString operator== re-reads `m_pStringData` at every use (no cached block locals), left operand first in the
// identity test: VC7.1 then CSEs the right operand's ADDRESS (`last - 1`) into a temporary emitted as its own
// `sub eax,4 ; mov eax,[eax]`; with cached block locals the single load folds into `[eax-4]` (solved by the shared
// retry advisor).
struct CCharStringData {
    const char*   m_pChars;                                 // +0x00
    unsigned long m_length;                                 // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
};
int __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

static __forceinline bool operator==(const CCharString& sa, const CCharString& sb)
{
    if (sa.m_pStringData == sb.m_pStringData)
        return true;
    if (!sa.m_pStringData || !sb.m_pStringData)
        return false;
    if (sa.m_pStringData->m_length != sb.m_pStringData->m_length)
        return false;
    return Compare_00411570(sa.m_pStringData->m_pChars, sb.m_pStringData->m_pChars) == 0;
}

struct CCharStringVector {                                  // retail std::vector<CCharString>, 12 bytes
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    CCharString& back() { return *(m_pLast - 1); }
};

enum ETradeRoutes { TRADE_ROUTE_0, TRADE_ROUTE_1, TRADE_ROUTE_2 };

struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    void*         GSI;                                      // +0x40
    unsigned char _pad44[4];                                // +0x44
};

struct CQR_EscortTraderScript : CQuestScriptBase {
    unsigned char     _pad48[0x7c];                         // +0x48
    ETradeRoutes      TradeRoute;                           // +0xc4
    CCharStringVector RouteDynamic[3];                      // +0xc8
    CCharStringVector RouteStatic[3];                       // +0xec
    CCharString       PreviousLoadedRegion;                 // +0x110
    CCharString       CurrentLoadedRegion;                  // +0x114
    bool InFinalRegion();
};

bool CQR_EscortTraderScript::InFinalRegion()
{
    return CurrentLoadedRegion == RouteStatic[TradeRoute].back();
}

// ---- stubs and driver ----
void CScriptBase::v00() {}
static int g_compares = 0; static const char* g_ca; static const char* g_cb;
int __fastcall Compare_00411570(const char* a, const char* b) { ++g_compares; g_ca = a; g_cb = b; return strcmp(a, b); }
int main()
{
    static CCharStringData d1 = { "GUILD", 5 }, d2 = { "OAKVALE", 7 }, d3 = { "OAKVALE", 7 }, d4 = { "BOWERS", 6 };
    CCharString route0[2], route1[3];
    route0[0].m_pStringData = &d1; route0[1].m_pStringData = &d2;
    route1[0].m_pStringData = &d4; route1[1].m_pStringData = &d1; route1[2].m_pStringData = 0;
    static unsigned char store[sizeof(CQR_EscortTraderScript)]; memset(store, 0, sizeof store);
    CQR_EscortTraderScript& s = *(CQR_EscortTraderScript*)store;
    s.RouteStatic[0].m_pFirst = route0; s.RouteStatic[0].m_pLast = route0 + 2;
    s.RouteStatic[1].m_pFirst = route1; s.RouteStatic[1].m_pLast = route1 + 3;
    s.TradeRoute = TRADE_ROUTE_0;
    s.CurrentLoadedRegion.m_pStringData = &d2;                 // same block: no compare
    CHECK(s.InFinalRegion() == true); CHECK(g_compares == 0);
    s.CurrentLoadedRegion.m_pStringData = &d3;                 // equal text, other block
    CHECK(s.InFinalRegion() == true); CHECK(g_compares == 1); CHECK(g_ca == d3.m_pChars); CHECK(g_cb == d2.m_pChars);
    s.CurrentLoadedRegion.m_pStringData = &d1;                 // different length
    CHECK(s.InFinalRegion() == false); CHECK(g_compares == 1);
    s.TradeRoute = TRADE_ROUTE_1;                              // the last entry is an empty string
    CHECK(s.InFinalRegion() == false);
    s.CurrentLoadedRegion.m_pStringData = 0;
    CHECK(s.InFinalRegion() == true);
    if (fails == 0) printf("00e24210_TEST PASS\n"); else printf("00e24210_TEST FAIL\n");
    return fails;
}