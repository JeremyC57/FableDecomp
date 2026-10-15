// Behaviour test for CTCBoastingPosition::DeactivatePosition (0x00781FE0): flags cleared, live particle killed, owner hidden,
// interface-9 component disabled.
#include <cstdio>
struct Comp9 { bool Enabled; };
struct Thing {
    unsigned char Flags91; int kills; int hidden; unsigned int bits[9]; Comp9* c9;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool now) { if (now) ++kills; }
    void SetVisible(bool v) { if (!v) ++hidden; }
    bool GetInterface(Comp9** o, int t) { if (bits[t >> 5] & (1u << (t & 31))) { *o = c9; return true; } return false; }
};
struct IPtr { Thing* item; Thing* GetPItem() const { return item; } };
struct Pos {
    Thing* ParentThing; bool PositionActivated; bool BoastOn; IPtr PParticleEffect;
    void KillParticleEffect() { if (PParticleEffect.GetPItem() && !PParticleEffect.GetPItem()->IsDead()) PParticleEffect.GetPItem()->Kill(true); }
    void DeactivatePosition()
    {
        PositionActivated = false; BoastOn = false; KillParticleEffect();
        ParentThing->SetVisible(false);
        Comp9* c; if (ParentThing->GetInterface(&c, 9)) c->Enabled = false;
    }
};
int main()
{
    Comp9 c9; c9.Enabled = true; Thing owner; owner.kills = 0; owner.hidden = 0; owner.Flags91 = 0; owner.c9 = &c9;
    for (int i = 0; i < 9; ++i) owner.bits[i] = 0; owner.bits[0] = 1u << 9;
    Thing fx; fx.Flags91 = 0; fx.kills = 0;
    Pos p; p.ParentThing = &owner; p.PositionActivated = true; p.BoastOn = true; p.PParticleEffect.item = &fx;
    p.DeactivatePosition();
    if (p.PositionActivated || p.BoastOn || fx.kills != 1 || owner.hidden != 1 || c9.Enabled) { std::printf("00781fe0_TEST FAIL\n"); return 1; }
    fx.Flags91 = 1; p.DeactivatePosition();
    if (fx.kills != 1) { std::printf("00781fe0_TEST FAIL\n"); return 1; }
    std::printf("00781fe0_TEST PASS\n");
    return 0;
}