// Behaviour test for 0x00E245E0 NScript::CQR_EscortTraderScript::TravellingWrongPath(): a current region on the trade
// route's dynamic list is on course (and the previous region, if listed, is struck off that list, first match only);
// otherwise, after a wrong turn was already detected, a current region on the static list is back on course; anything
// else flags a wrong turn and returns true. Fixed-size stand-ins for the STLport vectors; CCharString compares by text.
#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text = "") : PStringData(text) { }
    bool operator==(const CCharString& o) const { return std::strcmp(PStringData, o.PStringData) == 0; }
};
class CStringVector { public:                              // the test's stand-in for std::vector<CCharString>
    typedef CCharString* iterator;
    CCharString Items[8];
    unsigned int Count;
    CStringVector() : Count(0) { }
    iterator begin() { return Items; }
    iterator end() { return Items + Count; }
    iterator erase(iterator pos) { for (iterator p = pos; p + 1 != end(); ++p) *p = *(p + 1); --Count; return pos; }
    void Set(const char* a, const char* b, const char* c) { Count = 0; Items[Count++] = a; Items[Count++] = b; if (c) Items[Count++] = c; }
};
namespace NScript {
class CQR_EscortTraderScript { public:
    bool          WrongTurnDetected;
    long          TradeRoute;
    CStringVector RouteDynamic[3];
    CStringVector RouteStatic[3];
    CCharString   PreviousLoadedRegion;
    CCharString   CurrentLoadedRegion;
    bool TravellingWrongPath();
};

bool CQR_EscortTraderScript::TravellingWrongPath()
{
    CStringVector::iterator it;
    for (it = RouteDynamic[TradeRoute].begin(); it != RouteDynamic[TradeRoute].end(); ++it) {
        if (*it == CurrentLoadedRegion) {
            for (it = RouteDynamic[TradeRoute].begin(); it != RouteDynamic[TradeRoute].end(); ++it) {
                if (*it == PreviousLoadedRegion) {
                    RouteDynamic[TradeRoute].erase(it);
                    break;
                }
            }
            WrongTurnDetected = false;
            return false;
        }
    }
    if (WrongTurnDetected) {
        for (it = RouteStatic[TradeRoute].begin(); it != RouteStatic[TradeRoute].end(); ++it) {
            if (*it == CurrentLoadedRegion) {
                WrongTurnDetected = false;
                return false;
            }
        }
    }
    WrongTurnDetected = true;
    return true;
}
}   // namespace NScript

int main()
{
    int ok = 1;
    NScript::CQR_EscortTraderScript s;
    s.TradeRoute = 1;
    s.RouteDynamic[0].Set("X", "Y", 0);
    s.RouteStatic[0].Set("X", "Y", 0);
    s.RouteDynamic[1].Set("Oakvale", "Greatwood", "Bowerstone");
    s.RouteStatic[1].Set("Oakvale", "Greatwood", "Bowerstone");
    // on course, coming from a listed region: the previous region is erased
    s.WrongTurnDetected = true; s.PreviousLoadedRegion = "Oakvale"; s.CurrentLoadedRegion = "Greatwood";
    ok = ok && !s.TravellingWrongPath() && !s.WrongTurnDetected && s.RouteDynamic[1].Count == 2
            && s.RouteDynamic[1].Items[0] == CCharString("Greatwood") && s.RouteDynamic[0].Count == 2;
    // on course, previous region not listed: nothing erased
    s.PreviousLoadedRegion = "Elsewhere";
    ok = ok && !s.TravellingWrongPath() && s.RouteDynamic[1].Count == 2;
    // off the dynamic list, no wrong turn yet: a wrong turn is flagged even though the static list has it
    s.CurrentLoadedRegion = "Oakvale"; s.WrongTurnDetected = false;
    ok = ok && s.TravellingWrongPath() && s.WrongTurnDetected;
    // wrong turn already detected, region on the static list: back on course
    ok = ok && !s.TravellingWrongPath() && !s.WrongTurnDetected;
    // a region on neither list
    s.CurrentLoadedRegion = "Hook"; s.WrongTurnDetected = true;
    ok = ok && s.TravellingWrongPath() && s.WrongTurnDetected && s.RouteDynamic[1].Count == 2 && s.RouteStatic[1].Count == 3;
    // the route index selects the lists
    s.TradeRoute = 0; s.CurrentLoadedRegion = "Y"; s.PreviousLoadedRegion = "X";
    ok = ok && !s.TravellingWrongPath() && s.RouteDynamic[0].Count == 1 && s.RouteDynamic[1].Count == 2;
    if (!ok) { std::printf("00e245e0_TEST FAIL\n"); return 1; }
    std::printf("00e245e0_TEST PASS\n");
    return 0;
}