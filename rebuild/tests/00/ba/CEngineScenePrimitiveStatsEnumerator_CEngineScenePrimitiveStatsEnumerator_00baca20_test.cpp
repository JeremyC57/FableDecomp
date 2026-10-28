// Behaviour test for CEngineScenePrimitiveStatsEnumerator::CEngineScenePrimitiveStatsEnumerator (0x00BACA20): destination map, area box, stat index and flags stored.
#include <cstdio>
struct Map { int dummy; };
struct Box { float TLX, TLY, BRX, BRY; };
struct Enumerator { Map* Destination; Box Area; int StatIndex; unsigned int StatFlags;
    Enumerator(Map& d, const Box& a, int i, unsigned int f) : Destination(&d), Area(a), StatIndex(i), StatFlags(f) {} };
int main()
{
    Map m; Box b = { 1.0f, 2.0f, 3.0f, 4.0f }; Enumerator e(m, b, 6, 0x80u);
    if (e.Destination != &m || e.Area.TLX != 1.0f || e.Area.BRY != 4.0f || e.StatIndex != 6 || e.StatFlags != 0x80u) { std::printf("00baca20_TEST FAIL\n"); return 1; }
    std::printf("00baca20_TEST PASS\n");
    return 0;
}