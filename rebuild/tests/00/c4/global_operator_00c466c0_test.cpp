// Behaviour test for 00c466c0: operator<(SRegionLookupKey, SRegionLookupKey) -- by Length, ties by Position (X then Y)
#include <cstdio>

struct C2DVector { float X, Y; };
static bool Less(const C2DVector& a, const C2DVector& b) { if (a.X == b.X) return a.Y < b.Y; return a.X < b.X; }
struct SRegionLookupKey { C2DVector Position; float Length; };
static bool Less(const SRegionLookupKey& a, const SRegionLookupKey& b)
{
    if (a.Length == b.Length)
        return Less(a.Position, b.Position);
    return a.Length < b.Length;
}

int main()
{
    SRegionLookupKey k1 = { { 3.0f, 4.0f }, 5.0f };
    SRegionLookupKey k2 = { { 4.0f, 3.0f }, 5.0f };
    SRegionLookupKey k3 = { { 0.0f, 1.0f }, 1.0f };
    if (!Less(k1, k2) || Less(k2, k1) || !Less(k3, k1) || Less(k1, k3) || Less(k1, k1)) { std::printf("00c466c0_TEST FAIL\n"); return 1; }
    std::printf("00c466c0_TEST PASS\n");
    return 0;
}