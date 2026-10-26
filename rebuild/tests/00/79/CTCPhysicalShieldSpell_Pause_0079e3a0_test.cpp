#include <cstdio>
// Behaviour model of CTCPhysicalShieldSpell::Pause (0x0079E3A0)
class CThing { public: unsigned char Flags91; int kills; void Kill(bool) { ++kills; } };
struct CPtr { CThing* p; CThing* GetPItem() const { return p; } };
static bool IsThingAlive(const CPtr& p) { return p.GetPItem() && !(p.GetPItem()->Flags91 & 1); }
static unsigned long g_stopped;
class CTCPhysicalShieldSpell { public:
    CPtr ShieldEffect, SwirlyEffect; unsigned long LoopingSound; bool Pausable, Paused;
    void StopSound(unsigned long s) { g_stopped = s; }
    void Pause() {
        if (Pausable && !Paused) {
            Paused = true; StopSound(LoopingSound);
            if (IsThingAlive(ShieldEffect)) ShieldEffect.GetPItem()->Kill(true);
            if (IsThingAlive(SwirlyEffect)) SwirlyEffect.GetPItem()->Kill(true);
        }
    } };
int main()
{
    CThing a = { 0, 0 }, b = { 1, 0 };
    CTCPhysicalShieldSpell s = { { &a }, { &b }, 77, true, false };
    s.Pause();
    if (!s.Paused || g_stopped != 77 || a.kills != 1 || b.kills != 0) { std::printf("0079e3a0_TEST FAIL\n"); return 1; }
    s.Pause();
    if (a.kills != 1) { std::printf("0079e3a0_TEST FAIL\n"); return 1; }
    std::printf("0079e3a0_TEST PASS\n");
    return 0;
}