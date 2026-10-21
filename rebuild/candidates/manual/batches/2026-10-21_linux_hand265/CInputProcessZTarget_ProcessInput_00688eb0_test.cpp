#include <cstdio>
// Behaviour test for CInputProcessZTarget::ProcessInput (0x00688EB0): press locks the creature's current target and arms the
// hold timer; held with the same target disarms the timer and deactivates the Z-target once.
struct CThing;
struct CTargeting { CThing* Target; CThing* GetTargetThing() { return Target; } };
struct CThing {
    bool Dead; CTargeting* Targeting;
    bool GetInterface(CTargeting** out, long id) const { if (id != 8 || !Targeting) return false; *out = Targeting; return true; }
};
struct CPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
static bool IS_THING_ALIVE(const CPointer& p) { return p.GetPItem() && !p.GetPItem()->Dead; }
struct CEvent { bool Pressed, Held; long Action; };
struct CProcessed { int Handled; void SetHandled() { Handled++; } };
struct CZTarget {
    bool Enabled; CPointer PCreature; long DeactivateHoldTimer; CPointer PTarget; int Deactivations;
    void DeactivateZTarget(CProcessed&) { Deactivations++; }
    void ProcessInput(const CEvent& event, CProcessed& processed)
    {
        if (!Enabled) return;
        if (event.Pressed && event.Action == 0x4a) {
            CThing* target = 0;
            CTargeting* targeting;
            if (IS_THING_ALIVE(PCreature) && PCreature.GetPItem()->GetInterface(&targeting, 8)) target = targeting->GetTargetThing();
            { CPointer p; p.SetPItem(target); PTarget.SetPItem(p.GetPItem()); }
            DeactivateHoldTimer = 15;
            processed.SetHandled();
        }
        if (event.Held && event.Action == 0x4a) {
            CTargeting* targeting;
            if (IS_THING_ALIVE(PCreature) && PCreature.GetPItem()->GetInterface(&targeting, 8)) {
                if (!(PTarget.GetPItem() != targeting->GetTargetThing()) && DeactivateHoldTimer != -1) {
                    DeactivateHoldTimer = -1;
                    DeactivateZTarget(processed);
                }
            }
        }
    }
};
static int Fail() { std::printf("00688eb0_TEST FAIL\n"); return 1; }
int main()
{
    CThing enemy = { false, 0 };
    CTargeting tg = { &enemy };
    CThing hero = { false, &tg };
    CZTarget z = { true, { &hero }, 0, { 0 }, 0 };
    CProcessed p = { 0 };
    CEvent press = { true, false, 0x4a }, held = { false, true, 0x4a }, other = { true, true, 0x10 };
    z.ProcessInput(press, p);
    if (z.PTarget.Item != &enemy || z.DeactivateHoldTimer != 15 || p.Handled != 1) return Fail();
    z.ProcessInput(held, p);
    if (z.DeactivateHoldTimer != -1 || z.Deactivations != 1) return Fail();
    z.ProcessInput(held, p);
    if (z.Deactivations != 1) return Fail();
    z.ProcessInput(other, p);
    if (p.Handled != 1) return Fail();
    hero.Dead = true;
    z.ProcessInput(press, p);
    if (z.PTarget.Item != 0 || p.Handled != 2) return Fail();
    z.Enabled = false; z.DeactivateHoldTimer = 3;
    z.ProcessInput(press, p);
    if (z.DeactivateHoldTimer != 3) return Fail();
    std::printf("00688eb0_TEST PASS\n");
    return 0;
}
