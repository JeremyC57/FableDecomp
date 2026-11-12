// Behaviour test for CTCBurntEffect::FrameUpdate 0x007CD9A0: model of the fade-in / kill / fade-out / remove logic.
#include <cstdio>
#include <vector>
struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct Thing { bool dead; Colour tint; bool hasApp; int removed; int killed;
    Thing() : dead(false), tint(0,0,0,0), hasApp(true), removed(0), killed(0) {} };
struct Effect {
    Thing* parent; long def[4]; long CurTime, FadeInTime, FadeOutTimeStart, FadeOutTime;
    std::vector<Thing*> Effects, TintList;
    void Tint(float amount)
    {
        float white = (float)(long)((1.0f - amount) * 255.0f);
        Colour c((unsigned char)(long)(def[0] * amount + white), (unsigned char)(long)(def[1] * amount + white),
                 (unsigned char)(long)(def[2] * amount + white), (unsigned char)(long)(def[3] * amount + white));
        if (parent->hasApp) parent->tint = c;
        for (size_t i = 0; i < TintList.size(); ++i) { Thing* t = TintList[i]; if (t && !t->dead && t->hasApp) t->tint = c; }
    }
    void FrameUpdate()
    {
        if (CurTime <= FadeInTime) {
            float amount = (float)CurTime / FadeInTime; if (FadeInTime == 0) amount = 1.0f; Tint(amount);
        } else if (CurTime >= FadeOutTimeStart) {
            if (CurTime == FadeOutTimeStart)
                for (unsigned i = 0; i < Effects.size(); ++i) if (Effects[i] && !Effects[i]->dead) Effects[i]->killed++;
            float amount = 1.0f - (float)(CurTime - FadeOutTimeStart) / FadeOutTime; if (FadeOutTime == 0) amount = 1.0f; Tint(amount);
        }
        ++CurTime;
        if (CurTime > FadeOutTimeStart + FadeOutTime) parent->removed++;
    }
};
int main()
{
    Thing p, a, b, e1, e2; b.dead = true; e2.dead = true;
    Effect fx; fx.parent = &p; fx.def[0] = 200; fx.def[1] = 100; fx.def[2] = 50; fx.def[3] = 255;
    fx.CurTime = 0; fx.FadeInTime = 2; fx.FadeOutTimeStart = 4; fx.FadeOutTime = 2;
    fx.TintList.push_back(&a); fx.TintList.push_back(&b); fx.Effects.push_back(&e1); fx.Effects.push_back(&e2);
    fx.FrameUpdate();   // amount 0 -> white
    int bad = 0;
    if (p.tint.R != 255 || p.tint.G != 255 || p.tint.B != 255 || p.tint.A != 255 || a.tint.R != 255 || b.tint.R != 0) bad = 1;
    fx.FrameUpdate(); fx.FrameUpdate();   // CurTime 2 -> amount 1 -> full colour
    if (p.tint.R != 200 || p.tint.G != 100 || p.tint.B != 50 || p.tint.A != 255) bad = 2;
    fx.FrameUpdate();   // CurTime 3: between fade in and out, nothing
    if (e1.killed != 0) bad = 3;
    fx.FrameUpdate();   // CurTime 4: kill live effects, amount 1
    if (e1.killed != 1 || e2.killed != 0 || p.tint.R != 200) bad = 4;
    fx.FrameUpdate();   // CurTime 5: amount 0.5
    if (p.tint.R != 227 || p.removed != 0) bad = 5;   // 200*.5 + 127 = 227
    fx.FrameUpdate();   // CurTime 6 -> 7 > 6: removed
    if (p.removed != 1 || p.tint.R != 255) bad = 6;
    if (bad) { std::printf("007cd9a0_TEST FAIL\n"); return 1; }
    std::printf("007cd9a0_TEST PASS\n");
    return 0;
}