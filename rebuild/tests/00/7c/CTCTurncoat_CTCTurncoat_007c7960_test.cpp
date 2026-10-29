// Behaviour test for CTCTurncoat::~CTCTurncoat @ 0x007C7960: kill a live turncoated effect, then the members go in reverse order.
#include <stdio.h>
#include <string.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); }
class CThing { public: bool Dead; int Kills; bool LastImmediate;
    bool IsDead() const { return Dead; }
    void Kill(bool immediately) { Kills++; LastImmediate = immediately; } };
struct CIntelligentPointer { CThing* P; const char* Tag;
    CIntelligentPointer(CThing* p, const char* tag) : P(p), Tag(tag) { }
    ~CIntelligentPointer() { Log(Tag); }
    CThing* Get() const { return P; }
    CThing* operator->() const { return P; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { if (p.Get() != 0) { if (!p->IsDead()) return true; } return false; }
struct CDefPointer { int* RefCount; ~CDefPointer() { if (RefCount) { --*RefCount; Log("D"); } } };
struct CStageEffects { ~CStageEffects() { Log("V"); } };
class CTCBase { public: virtual ~CTCBase() { Log("B"); } };
class CTCTurncoat : public CTCBase { public:
    CDefPointer PDef; CIntelligentPointer TurncoatedEffect; CIntelligentPointer TurncoatOriginator; CStageEffects TurncoatStageEffects;
    CTCTurncoat(int* rc, CThing* effect) : TurncoatedEffect(effect, "E"), TurncoatOriginator(0, "O") { PDef.RefCount = rc; }
    ~CTCTurncoat() { if (IS_THING_ALIVE(TurncoatedEffect)) TurncoatedEffect->Kill(true); }
};
static int fail(int line) { printf("007c7960_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    int rc = 2;
    CThing live = { false, 0, false }, dead = { true, 0, false };
    { CTCTurncoat* t = new CTCTurncoat(&rc, &live); g_log[0] = 0; delete t; }
    if (live.Kills != 1 || !live.LastImmediate || rc != 1 || strcmp(g_log, "VOEDB") != 0) return fail(__LINE__);
    { CTCTurncoat* t = new CTCTurncoat(&rc, &dead); delete t; }
    if (dead.Kills != 0 || rc != 0) return fail(__LINE__);
    { CTCTurncoat* t = new CTCTurncoat(0, 0); delete t; }
    printf("007c7960_TEST PASS\n");
    return 0;
}