// Behaviour test for CTCBase::GetThingSearchTools 0x00686D80: a thing component's search tools are the current thing manager's. The body ignores `this` and forwards to the current thing manager
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
struct CTCBase { int own; CThingSearchTools& GetThingSearchTools() { return PCurrentThingManager->GetThingSearchTools(); } };
static int Key(const CThingSearchTools& t) { return t.id; }
static int Key(long v) { return (int)v; }
static int Key(bool b) { return b ? 1 : 0; }
int main()
{
    CThingManager a = { { 11 }, 31, true }, b = { { 22 }, 32, false };
    CTCBase x, y; x.own = 1; y.own = 2;
    PCurrentThingManager = &a;
    int ka = Key(x.GetThingSearchTools());
    if (Key(y.GetThingSearchTools()) != ka) { std::printf("00686d80_TEST FAIL\n"); return 1; }
    PCurrentThingManager = &b;
    int kb = Key(x.GetThingSearchTools());
    if (ka == kb) { std::printf("00686d80_TEST FAIL\n"); return 1; }
    std::printf("00686d80_TEST PASS\n");
    return 0;
}