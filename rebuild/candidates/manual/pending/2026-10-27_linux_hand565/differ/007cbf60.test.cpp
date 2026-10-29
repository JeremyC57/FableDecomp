// Behaviour test for CTCMultiArrow::~CTCMultiArrow @ 0x007CBF60: a live bow effect and every live ammo effect are killed.
#include <stdio.h>
struct CThing { bool Dead; int Kills; bool IsDead() const { return Dead; } void Kill(bool) { Kills++; } };
struct CIntelligentPointer { CThing* P; CThing* Get() const { return P; } CThing* operator->() const { return P; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { if (p.Get() != 0) { if (!p->IsDead()) return true; } return false; }
struct CVec { CIntelligentPointer* Start; CIntelligentPointer* Finish; unsigned int size() const { return (unsigned int)(Finish - Start); }
    CIntelligentPointer& operator[](unsigned int n) { return *(Start + n); } };
struct CTCMultiArrow { CVec AmmoEffects; CIntelligentPointer BowEffect;
    void Destroy() {
        if (IS_THING_ALIVE(BowEffect)) BowEffect->Kill(true);
        for (long i = 0; i < (long)AmmoEffects.size(); ++i) if (IS_THING_ALIVE(AmmoEffects[i])) AmmoEffects[i]->Kill(true);
    } };
int main()
{
    CThing bow = { false, 0 }, a = { false, 0 }, b = { true, 0 };
    CIntelligentPointer items[3] = { { &a }, { 0 }, { &b } };
    CTCMultiArrow m; m.AmmoEffects.Start = items; m.AmmoEffects.Finish = items + 3; m.BowEffect.P = &bow;
    m.Destroy();
    if (bow.Kills != 1 || a.Kills != 1 || b.Kills != 0) { printf("007cbf60_TEST FAIL\n"); return 1; }
    printf("007cbf60_TEST PASS\n");
    return 0;
}
