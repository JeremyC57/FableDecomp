// Behaviour test for CTCBoastingPosition::RemoveBoast (0x00782060): boast flag cleared, live particle killed, interface-9
// component disabled; PositionActivated untouched.
#include <cstdio>
struct Comp9 { bool Enabled; };
struct Thing {
    unsigned char Flags91; int kills; unsigned int bits[9]; Comp9* c9;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool now) { if (now) ++kills; }
    bool GetInterface(Comp9** o, int t) { if (bits[t >> 5] & (1u << (t & 31))) { *o = c9; return true; } return false; }
};
struct IPtr { Thing* item; Thing* GetPItem() const { return item; } };
struct Pos {
    Thing* ParentThing; bool PositionActivated; bool BoastOn; IPtr PParticleEffect;
    void KillParticleEffect() { if (PParticleEffect.GetPItem() && !PParticleEffect.GetPItem()->IsDead()) PParticleEffect.GetPItem()->Kill(true); }
    void RemoveBoast() { BoastOn = false; KillParticleEffect(); Comp9* c; if (ParentThing->GetInterface(&c, 9)) c->Enabled = false; }
};
int main()
{
    Comp9 c9; c9.Enabled = true; Thing owner; owner.Flags91 = 0; owner.kills = 0; owner.c9 = &c9;
    for (int i = 0; i < 9; ++i) owner.bits[i] = 0;
    Pos p; p.ParentThing = &owner; p.PositionActivated = true; p.BoastOn = true; p.PParticleEffect.item = 0;
    p.RemoveBoast();
    if (p.BoastOn || !p.PositionActivated || !c9.Enabled) { std::printf("00782060_TEST FAIL\n"); return 1; }
    owner.bits[0] = 1u << 9; p.PParticleEffect.item = &owner; p.RemoveBoast();
    if (c9.Enabled || owner.kills != 1) { std::printf("00782060_TEST FAIL\n"); return 1; }
    std::printf("00782060_TEST PASS\n");
    return 0;
}