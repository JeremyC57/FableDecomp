// Behaviour test for CTCBurntEffect::~CTCBurntEffect @ 0x007CDC20: every live effect is faded or killed, dead or empty ones are skipped.
#include <stdio.h>
class CThing { public: bool Dead; int Faded; bool IsDead() const { return Dead; } };
struct CIntelligentPointer { CThing* P; CThing* Get() const { return P; } CThing* operator->() const { return P; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { if (p.Get() != 0) { if (!p->IsDead()) return true; } return false; }
static int g_calls = 0;
class CTCSpecialAbilities { public: static void FadeOrKillEffect(const CIntelligentPointer& effect) { g_calls++; effect.P->Faded++; } };
struct CEffects { CIntelligentPointer* Start; CIntelligentPointer* Finish;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    CIntelligentPointer& operator[](unsigned int n) { return *(Start + n); } };
class CTCBurntEffect { public: CEffects Effects;
    void Destroy() { for (unsigned int i = 0; i < Effects.size(); ++i) { if (IS_THING_ALIVE(Effects[i])) CTCSpecialAbilities::FadeOrKillEffect(Effects[i]); } } };
int main()
{
    CThing a = { false, 0 }, b = { true, 0 }, c = { false, 0 };
    CIntelligentPointer items[4] = { { &a }, { &b }, { 0 }, { &c } };
    CTCBurntEffect e; e.Effects.Start = items; e.Effects.Finish = items + 4;
    e.Destroy();
    if (g_calls != 2 || a.Faded != 1 || b.Faded != 0 || c.Faded != 1) { printf("007cdc20_TEST FAIL\n"); return 1; }
    e.Effects.Finish = items; e.Destroy();
    if (g_calls != 2) { printf("007cdc20_TEST FAIL\n"); return 1; }
    printf("007cdc20_TEST PASS\n");
    return 0;
}