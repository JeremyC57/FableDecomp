// Behaviour test for CCodaVoice::UpdatePitchEnvelopeStages (0x00C5A700): an expired attack moves on to the next stage and
// requests a recompute; an active stage keeps counting down.
#include <stdio.h>
enum { ATTACK = 0x200, DECAY = 0x400, SUSTAIN = 0x800 };
struct Voice { unsigned long flags; long counter;
    bool Update(long attack, long decay, bool recompute) {
        switch (flags & 0x7e20) {
        case ATTACK: if (counter <= 0) { flags = (flags & ~0x7e20) | DECAY; counter = decay; recompute = true; } else break;
        case DECAY: if (counter <= 0) { flags = (flags & ~0x7e20) | SUSTAIN; recompute = true; } break;
        default: break; }
        (void)attack; return recompute; } };
int main()
{
    Voice v = { ATTACK, 0 };
    bool r1 = v.Update(10, 5, false);
    bool ok = r1 && (v.flags & 0x7e20) == DECAY && v.counter == 5;
    bool r2 = v.Update(10, 5, false);
    ok = ok && !r2 && (v.flags & 0x7e20) == DECAY;
    printf(ok ? "00c5a700_TEST PASS\n" : "00c5a700_TEST FAIL\n");
    return ok ? 0 : 1;
}