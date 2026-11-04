// Behaviour test for CThing::PeekThingSearchTools 0x004C7A60: the const search tools are the current thing manager's. The body ignores `this` and forwards to the current thing manager
// (the static CThing::PCurrentThingManager, retail 0x013B8A1C), so two different objects see the same answer and switching the
// current manager switches the answer.
#include <cstdio>
struct CThingSearchTools { int id; };
struct CThingManager {
    CThingSearchTools Tools; long SerialiseVersion; bool SerialisingGameState;
    CThingSearchTools& GetThingSearchTools() { return Tools; }
    const CThingSearchTools& PeekThingSearchTools() const { return Tools; }
    long GetSerialiseVersion() const { return SerialiseVersion; }
    bool IsSerialisingGameState() const { return SerialisingGameState; }
};
static CThingManager* PCurrentThingManager = 0;
struct CThing { int own; const CThingSearchTools& PeekThingSearchTools() const { return PCurrentThingManager->PeekThingSearchTools(); } };
static int Key(const CThingSearchTools& t) { return t.id; }
static int Key(long v) { return (int)v; }
static int Key(bool b) { return b ? 1 : 0; }
int main()
{
    CThingManager a = { { 11 }, 31, true }, b = { { 22 }, 32, false };
    CThing x, y; x.own = 1; y.own = 2;
    PCurrentThingManager = &a;
    int ka = Key(x.PeekThingSearchTools());
    if (Key(y.PeekThingSearchTools()) != ka) { std::printf("004c7a60_TEST FAIL\n"); return 1; }
    PCurrentThingManager = &b;
    int kb = Key(x.PeekThingSearchTools());
    if (ka == kb) { std::printf("004c7a60_TEST FAIL\n"); return 1; }
    std::printf("004c7a60_TEST PASS\n");
    return 0;
}