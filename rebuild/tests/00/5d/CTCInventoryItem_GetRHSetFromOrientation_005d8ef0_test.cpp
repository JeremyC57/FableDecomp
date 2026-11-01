// Test for CTCInventoryItem::GetRHSetFromOrientation (0x005D8EF0): each of the 12 inventory orientations returns its
// own constant right-handed set by value; anything out of range returns the default orientation.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward, Up; };
static CRightHandedSet Make(float k) { CRightHandedSet s; s.Forward.X = k; s.Forward.Y = 0; s.Forward.Z = 0; s.Up.X = 0; s.Up.Y = 0; s.Up.Z = k; return s; }
static CRightHandedSet g_sets[12];
static CRightHandedSet GetRHSetFromOrientation(int o)
{
    switch (o) {
    case 0: return g_sets[0]; case 1: return g_sets[1]; case 2: return g_sets[2]; case 3: return g_sets[3];
    case 4: return g_sets[4]; case 5: return g_sets[5]; case 6: return g_sets[6]; case 7: return g_sets[7];
    case 8: return g_sets[8]; case 9: return g_sets[9]; case 10: return g_sets[10]; case 11: return g_sets[11];
    default: return g_sets[0];
    }
}
int main()
{
    for (int i = 0; i < 12; ++i) g_sets[i] = Make((float)(i + 1));
    bool ok = true;
    for (int i = 0; i < 12; ++i) { CRightHandedSet s = GetRHSetFromOrientation(i); if (s.Forward.X != (float)(i + 1) || s.Up.Z != (float)(i + 1)) ok = false; }
    if (GetRHSetFromOrientation(12).Forward.X != 1.0f || GetRHSetFromOrientation(-1).Up.Z != 1.0f) ok = false;
    std::printf(ok ? "005d8ef0_TEST PASS\n" : "005d8ef0_TEST FAIL\n");
    return ok ? 0 : 1;
}