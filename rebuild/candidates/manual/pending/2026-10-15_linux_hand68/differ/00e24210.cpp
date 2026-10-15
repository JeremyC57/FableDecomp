// CQR_EscortTraderScript::InFinalRegion() -- is the currently loaded region the last region of the trade route's static route list
// Grade: DIFFER(66v68) -- retail materialises the back() address (mov eax,[ecx+eax*4] ; sub eax,4 ; mov eax,[eax]) and loads CurrentLoadedRegion after it; the lane folds it into mov eax,[edx-4]
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24210, length 68 bytes.
// Callees: Compare @ 0x00411570 (`__fastcall(a, b)`, landed) under the inlined CCharString operator== (same
// block, null blocks, lengths at +4, then Compare == 0 as `neg ; sbb ; inc`).
// Layout (Ego_r.pdb CQR_EscortTraderScript - 0x14, then - 0x18 after the six PDB 16-byte vectors): TradeRoute
// +0xc4, RouteDynamic[3] +0xc8, RouteStatic[3] +0xec (12-byte vectors {first, last, end}),
// CurrentLoadedRegion +0x114.
// Shape: `return CurrentLoadedRegion == RouteStatic[TradeRoute].back();` -- with the operator reading the
// right-hand block first this source gives the retail sequence except the folded `[edx-4]` load.
// Tried: operator reading the left block first (70: CurrentLoadedRegion kept in esi), back() through an
// iterator object with `--it` (66) and with `operator-(1)` (66). Lane gap: RTM vector-slot address
// materialisation (GOTCHAS wave 24l).
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
    CCharStringData* b = sb.m_pStringData;
    CCharStringData* a = sa.m_pStringData;
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
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
