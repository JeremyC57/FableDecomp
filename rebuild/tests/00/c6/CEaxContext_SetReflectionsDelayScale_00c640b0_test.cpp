// behaviour test for 0x00C640B0 CEaxContext::SetReflectionsDelayScale (model: bit 2 of the slot flags, mirrored for the primary environment slot)
#include <cstdio>
struct Ctx {
    long primary; unsigned long slotFlags[4]; unsigned long reverbFlags[4]; unsigned long committed; unsigned long dirty;
    long Set(unsigned long slot, long enable)
    {
        unsigned long& flags = reverbFlags[slot];
        if (enable != (long)((flags >> 2) & 1)) {
            if (enable) { flags |= 0x4; if (primary == (long)slot && (slotFlags[slot] & 1)) { committed |= 0x4; dirty |= 0x800000; } }
            else { flags &= ~0x4u; if (primary == (long)slot && (slotFlags[slot] & 1)) { committed &= ~0x4u; dirty |= 0x800000; } }
        }
        return 0;
    }
};
int main()
{
    Ctx c = { 1, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, 0, 0 };
    int ok = c.Set(1, 1) == 0 && c.reverbFlags[1] == 0x4 && c.committed == 0x4 && c.dirty == 0x800000;
    c.dirty = 0; c.Set(2, 1);
    ok = ok && c.reverbFlags[2] == 0x4 && c.dirty == 0;
    c.Set(1, 0);
    ok = ok && c.reverbFlags[1] == 0 && c.committed == 0;
    if (!ok) { std::printf("00c640b0_TEST FAIL\n"); return 1; }
    std::printf("00c640b0_TEST PASS\n");
    return 0;
}