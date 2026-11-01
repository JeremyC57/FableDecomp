// Test for CEaxContext::SetEnvironmentSize 0x00C677C0: the 1..100 range check (-3 with last error -2), the unchanged
// early return, the committed copy + dirty bit 2 on the primary environment slot, and the scaled reverb update under flag 8.
#include <cstdio>
struct Ctx {
    long lastError; unsigned long primary; unsigned long dirty; float size[2]; unsigned long flags[2]; long reverb[2];
    unsigned long slotFlags[2]; float committedSize; long committedReverb; float reverbBase[2]; float reverbSize[2];
    static float GainToMillibels(float g) { return g * 1000.0f; }   // stand-in with the same shape (monotonic)
    long SetEnvironmentSize(unsigned long slot, float value) {
        if (value < 1.0f || value > 100.0f) { lastError = -2; return -3; }
        float& stored = size[slot];
        if (stored == value) return 0;
        stored = value;
        if (primary == slot && (slotFlags[slot] & 1)) { committedSize = value; dirty |= 2; }
        unsigned long& f = flags[slot];
        if (f & 8) {
            float scale = 1.0f; if (!(f & 1)) scale = 1.5f;
            long r = (long)(reverbBase[slot] - GainToMillibels(value / reverbSize[slot]) * scale);
            if (r < -10000) r = -10000; else if (r > 2000) r = 2000;
            long& storedReverb = reverb[slot];
            if (storedReverb != r) { storedReverb = r; if (primary == slot && (slotFlags[slot] & 1)) { committedReverb = r; dirty |= 0x1000; } }
        }
        return 0;
    }
};
int main() {
    Ctx c = { 0, 0, 0, { 7.5f, 7.5f }, { 8, 0 }, { 0, 0 }, { 1, 0 }, 0.0f, 0, { 0.0f, 0.0f }, { 7.5f, 7.5f } };
    bool ok = c.SetEnvironmentSize(0, 0.5f) == -3 && c.lastError == -2;
    ok = ok && c.SetEnvironmentSize(0, 200.0f) == -3;
    ok = ok && c.SetEnvironmentSize(0, 7.5f) == 0 && c.dirty == 0;             // unchanged
    ok = ok && c.SetEnvironmentSize(0, 15.0f) == 0 && c.committedSize == 15.0f && (c.dirty & 2);
    ok = ok && c.reverb[0] == -3000 && c.committedReverb == -3000 && (c.dirty & 0x1000);   // 0 - 1000*(15/7.5)*1.5 = -3000
    c.flags[0] = 9; c.SetEnvironmentSize(0, 100.0f);
    ok = ok && c.reverb[0] == -10000;                                        // clamp at -10000 (scale 1.0)
    ok = ok && c.SetEnvironmentSize(1, 20.0f) == 0 && c.size[1] == 20.0f && c.committedSize == 100.0f;   // slot 1 not primary
    if (ok) printf("00c677c0_TEST PASS\n"); else printf("00c677c0_TEST FAIL\n");
    return ok ? 0 : 1;
}