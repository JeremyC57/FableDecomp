// behaviour test for 0x00C64140 CEaxContext::SetReverbScale (model: bit 3 of the slot flags, mirrored for the primary environment slot)
#include <cstdio>
struct Ctx {
    long primary; unsigned long slotFlags[4]; unsigned long reverbFlags[4]; unsigned long committed; unsigned long dirty;
    long Set(unsigned long slot, long enable)
    {
        unsigned long& flags = reverbFlags[slot];
        if (enable != (long)((flags >> 3) & 1)) {
            if (enable) { flags |= 0x8; if (primary == (long)slot && (slotFlags[slot] & 1)) { committed |= 0x8; dirty |= 0x800000; } }
            else { flags &= ~0x8u; if (primary == (long)slot && (slotFlags[slot] & 1)) { committed &= ~0x8u; dirty |= 0x800000; } }
        }
        return 0;
    }
};
int main()
{
    Ctx c = { 1, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, 0, 0 };
    int ok = c.Set(1, 1) == 0 && c.reverbFlags[1] == 0x8 && c.committed == 0x8 && c.dirty == 0x800000;
    c.dirty = 0; c.Set(2, 1);
    ok = ok && c.reverbFlags[2] == 0x8 && c.dirty == 0;
    c.Set(1, 0);
    ok = ok && c.reverbFlags[1] == 0 && c.committed == 0;
    if (!ok) { std::printf("00c64140_TEST FAIL\n"); return 1; }
    std::printf("00c64140_TEST PASS\n");
    return 0;
}