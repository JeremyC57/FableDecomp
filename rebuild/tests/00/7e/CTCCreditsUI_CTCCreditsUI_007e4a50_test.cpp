// Test for 007e4a50 CTCCreditsUI::CTCCreditsUI: CTCBase(parent), inactive, no components, empty credit part, zero time.
#include <cstdio>
struct CThing { int id; };
static int g_baseCalls = 0;
struct CTCBase {
    CThing* ParentThing;
    CTCBase(CThing* parent) : ParentThing(parent) { ++g_baseCalls; }
    virtual ~CTCBase() {}
};
struct CCharString { const char* PStringData; CCharString() : PStringData("") {} };
struct CComponentVector { void* First; void* Last; void* End; CComponentVector() : First(0), Last(0), End(0) {} };
struct CTCCreditsUI : CTCBase {
    bool Active; CComponentVector Components; bool Quitting; CCharString CreditPart; double LastRenderTime;
    CTCCreditsUI(CThing* parent) : CTCBase(parent), Active(false), Quitting(false), LastRenderTime(0.0) {}
};
int main()
{
    CThing t = { 3 };
    CTCCreditsUI c(&t);
    if (g_baseCalls != 1 || c.ParentThing != &t || c.Active || c.Quitting || c.Components.First != 0 || c.Components.Last != 0
        || c.Components.End != 0 || c.CreditPart.PStringData[0] != 0 || c.LastRenderTime != 0.0) {
        std::printf("007e4a50_TEST FAIL\n"); return 1;
    }
    std::printf("007e4a50_TEST PASS\n");
    return 0;
}