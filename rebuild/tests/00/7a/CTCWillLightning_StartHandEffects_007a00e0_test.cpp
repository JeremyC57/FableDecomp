#include <cstdio>
// behaviour test: CTCWillLightning::StartHandEffects creates primary/secondary hand effects (swapped when left-handed) and attaches them to the hand bones
struct CThing { bool Dead; long Effect; bool HasEmitter; long AttachedHand; CThing* AttachedTo; };
static CThing g_things[8]; static int g_count = 0;
static CThing* CreateParticleEffect(long effect) { CThing* t = &g_things[g_count++]; t->Dead = false; t->Effect = effect; t->HasEmitter = true; t->AttachedHand = 0; t->AttachedTo = 0; return t; }
struct CPtr { CThing* Item; CPtr() : Item(0) {} };
static bool IsAlive(const CPtr& p) { return p.Item != 0 && !p.Item->Dead; }
struct CTCWillLightning {
    long WeaponState; bool LeftHandAnim; long Primary, Secondary, State;
    CPtr Player, Left, Right;
    bool IsRightHanded() const { switch (WeaponState) { case 0: return !LeftHandAnim; case 1: return false; } return true; }
    void StartHandEffects()
    {
        if (State == 6) return;
        if (IsRightHanded()) { Right.Item = CreateParticleEffect(Primary); Left.Item = CreateParticleEffect(Secondary); }
        else { Right.Item = CreateParticleEffect(Secondary); Left.Item = CreateParticleEffect(Primary); }
        if (IsAlive(Right) && Right.Item->HasEmitter) { Right.Item->AttachedTo = Player.Item; Right.Item->AttachedHand = 1; }
        if (IsAlive(Left)) { Left.Item->AttachedTo = Player.Item; Left.Item->AttachedHand = 2; }
    }
};
int main()
{
    CThing player; CTCWillLightning w; w.Player.Item = &player; w.Primary = 11; w.Secondary = 22;
    w.State = 6; w.WeaponState = 0; w.LeftHandAnim = false; w.StartHandEffects();
    bool ok = g_count == 0;
    w.State = 1; w.StartHandEffects();
    ok = ok && w.Right.Item->Effect == 11 && w.Left.Item->Effect == 22 && w.Right.Item->AttachedHand == 1 && w.Left.Item->AttachedTo == &player;
    w.WeaponState = 1; w.StartHandEffects();
    ok = ok && w.Right.Item->Effect == 22 && w.Left.Item->Effect == 11 && w.Left.Item->AttachedHand == 2;
    w.WeaponState = 0; w.LeftHandAnim = true; w.StartHandEffects();
    ok = ok && w.Right.Item->Effect == 22 && g_count == 6;
    if (!ok) { std::printf("007a00e0_TEST FAIL\n"); return 1; }
    std::printf("007a00e0_TEST PASS\n");
    return 0;
}