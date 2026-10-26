// behaviour test for 0x00C63B20 CEaxContext::SetEchoDepth: out-of-range values (0.0f..1.0f) fail with -3 and last error -2; an in-range change is stored and committed (dirty bit 0x10000) only for the primary slot with the environment flag
#include <cstdio>
struct Ctx { long primary; unsigned long slotFlags[4]; float rec[4]; float committed; unsigned long dirty; long lastError; };
static long Set(Ctx& c, unsigned long slot, float value)   // the function under test
{
    if (value < 0.0f || value > 1.0f) { c.lastError = -2; return -3; }
    float& stored = c.rec[slot];
    if (stored != value) {
        stored = value;
        if ((unsigned long)c.primary == slot && (c.slotFlags[slot] & 1)) { c.committed = value; c.dirty |= 0x10000; }
    }
    return 0;
}
int main()
{
    int fail = 0;
    Ctx c; c.primary = 1; c.dirty = 0; c.lastError = 0; c.committed = -1.0f;
    for (int i = 0; i < 4; ++i) { c.slotFlags[i] = 1; c.rec[i] = 0.0f; }
    float mid = (0.0f + 1.0f) / 2.0f;
    if (Set(c, 1, 1.0f * 2.0f + 1.0f) != -3 || c.lastError != -2) fail |= 1;
    c.lastError = 0;
    if (Set(c, 1, 0.0f - 1.0f) != -3 || c.lastError != -2) fail |= 2;
    if (Set(c, 0, mid) != 0 || c.rec[0] != mid || c.dirty != 0) fail |= 4;         // not the primary slot
    if (Set(c, 1, mid) != 0 || c.rec[1] != mid || c.committed != mid || c.dirty != 0x10000) fail |= 8;
    c.dirty = 0;
    if (Set(c, 1, mid) != 0 || c.dirty != 0) fail |= 16;                             // unchanged: nothing committed
    if (fail) { std::printf("00c63b20_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c63b20_TEST PASS\n");
    return 0;
}