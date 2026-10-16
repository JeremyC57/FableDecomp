// behaviour test for 0x007E33B0 CTCFlashOnHit::FrameUpdate
#include <cstdio>
struct Colour { unsigned char B, G, R, A; Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct Appearance { bool lit; int tints; int removed; unsigned char lastR, lastG;
    void SetSelfIlluminating(bool b) { lit = b; } void RemoveTintColour(void*) { ++removed; }
    void SetTintColour(const Colour& c, void*) { ++tints; lastR = c.R; lastG = c.G; } };
struct Thing { Appearance* app; int removedTC; void RemoveTC(long t) { removedTC = (int)t; } };
struct Flash {
    Thing* ParentThing; long Timer;
    void FrameUpdate()
    {
        ParentThing->app->SetSelfIlluminating(Timer > 0);
        if (Timer-- == 0) { ParentThing->app->RemoveTintColour(this); ParentThing->RemoveTC(0xfb); }
        else ParentThing->app->SetTintColour(Colour(255, 0, 0, 255), this);
    }
};
int main()
{
    Appearance a = { false, 0, 0, 0, 0 };
    Thing t = { &a, 0 };
    Flash f = { &t, 2 };
    f.FrameUpdate();
    bool ok = a.lit && a.tints == 1 && a.lastR == 255 && a.lastG == 0 && f.Timer == 1;
    f.FrameUpdate(); f.FrameUpdate();
    ok = ok && !a.lit && a.removed == 1 && t.removedTC == 0xfb && a.tints == 2 && f.Timer == -1;
    if (!ok) { std::printf("007e33b0_TEST FAIL\n"); return 1; }
    std::printf("007e33b0_TEST PASS\n");
    return 0;
}
