// Behaviour test for CTCKrakenTentacle::GetShouldStrike @ 0x007F1410 (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
class CThing { public: bool Dead; unsigned long modes; bool HasMode(unsigned long m) { return (modes & m) != 0; } };
static long g_worldFrame = 1000; static long g_fps = 30;
struct Model {
    CThing* ParentThing; float TentacleTimeBetweenAttacks; CThing* kraken; bool CanStrike; long LastStrikeTick;
    bool GetShouldStrike()
    {
        if (CanStrike && kraken && !kraken->Dead) {
            CThing* modes = ParentThing;
            if (!modes->HasMode(0x54) && !modes->HasMode(0x53)) {
                long fps = g_fps; long elapsed = g_worldFrame - LastStrikeTick;
                if ((float)elapsed > (float)fps * TentacleTimeBetweenAttacks) return true;
            }
        }
        return false;
    }
};
int main()
{
    bool ok = true;
    CThing owner = { false, 0 }; CThing kraken = { false, 0 };
    Model m = { &owner, 2.0f, &kraken, true, 900 };          // 100 frames since the strike, 60 needed
    ok = ok && m.GetShouldStrike();
    m.LastStrikeTick = 950; ok = ok && !m.GetShouldStrike();  // 50 frames: too soon
    m.LastStrikeTick = 940; ok = ok && !m.GetShouldStrike();  // exactly 60: not strictly more
    m.LastStrikeTick = 939; ok = ok && m.GetShouldStrike();
    owner.modes = 0x54; ok = ok && !m.GetShouldStrike(); owner.modes = 0x53; ok = ok && !m.GetShouldStrike(); owner.modes = 0;
    kraken.Dead = true; ok = ok && !m.GetShouldStrike(); kraken.Dead = false;
    m.kraken = 0; ok = ok && !m.GetShouldStrike(); m.kraken = &kraken;
    m.CanStrike = false; ok = ok && !m.GetShouldStrike();
    if (!ok) { printf("007f1410_TEST FAIL\n"); return 1; }
    printf("007f1410_TEST PASS\n"); return 0;
}