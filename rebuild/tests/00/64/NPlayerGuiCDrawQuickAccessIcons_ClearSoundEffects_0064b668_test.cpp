#include <cstdio>
// Behaviour test for 0064b668.

// CDrawQuickAccessIcons::ClearSoundEffects: with a charging sound playing and a live hero that has the sound interface, the sound
// is stopped and its ID forgotten; otherwise nothing changes.
struct CTCSound { unsigned int stopped; void StopSound(unsigned int id) { stopped = id; } };
class CThing { public: bool alive; bool hasSound; CTCSound sound; bool HasInterface(long t) { return t == 0x10 && hasSound; } CTCSound* GetInterfaceUnchecked(long t) { (void)t; return &sound; } };
static bool IS_THING_ALIVE_0043dbd0(CThing* const& t) { return t != 0 && t->alive; }
struct CDrawQuickAccessIcons {
    CThing* hero; unsigned int ChargingSoundID;
    void ClearSoundEffects()
    {
        if (ChargingSoundID > 0) {
            CThing* h = hero;
            if (IS_THING_ALIVE_0043dbd0(h)) {
                if (h->HasInterface(0x10)) { h->GetInterfaceUnchecked(0x10)->StopSound(ChargingSoundID); ChargingSoundID = 0; }
            }
        }
    }
};
int main()
{
    CThing hero; hero.alive = false; hero.hasSound = true; hero.sound.stopped = 0;
    CDrawQuickAccessIcons d; d.hero = &hero; d.ChargingSoundID = 7;
    d.ClearSoundEffects();
    int ok = d.ChargingSoundID == 7 && hero.sound.stopped == 0;
    hero.alive = true; hero.hasSound = false; d.ClearSoundEffects();
    ok = ok && d.ChargingSoundID == 7;
    hero.hasSound = true; d.ClearSoundEffects();
    ok = ok && d.ChargingSoundID == 0 && hero.sound.stopped == 7;
    if (!ok) { std::printf("0064b668_TEST FAIL\n"); return 1; }
    std::printf("0064b668_TEST PASS\n");
    return 0;
}