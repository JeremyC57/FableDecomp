// Behaviour test for CSurface::ReadOnlyLock (0x009F3010): a model of the row -- the lock adds the read-only flag (0x10) and returns
// the extents, pitch and memory; a failed lock returns an all-zero rect.
#include <stdio.h>
struct Rect { long Width, Height, Pitch; void* Bits; };
static unsigned long g_flags;
static bool Lock(unsigned long flags, bool succeed, long& pitch, void*& bits) { g_flags = flags; if (!succeed) return false; pitch = 256; bits = &g_flags; return true; }
static Rect ReadOnlyLock(unsigned long flags, long w, long h, bool succeed)
{
    long pitch; void* bits;
    if (Lock(flags | 0x10, succeed, pitch, bits)) { Rect r = { w, h, pitch, bits }; return r; }
    Rect z = { 0, 0, 0, 0 }; return z;
}
int main()
{
    Rect a = ReadOnlyLock(1, 64, 32, true); int ok = g_flags == 0x11 && a.Width == 64 && a.Height == 32 && a.Pitch == 256 && a.Bits;
    Rect b = ReadOnlyLock(0, 64, 32, false); ok = ok && b.Width == 0 && b.Height == 0 && b.Pitch == 0 && !b.Bits;
    printf(ok ? "009f3010_TEST PASS\n" : "009f3010_TEST FAIL\n");
    return ok ? 0 : 1;
}
