// Behaviour test for GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites> (0x00632830): a model of the row.
#include <stdio.h>

struct Map { long keys[8], vals[8]; int n; void insert(long k, long v) { for (int i = 0; i < n; ++i) if (keys[i] == k) return; keys[n] = k; vals[n++] = v; } };
static void ReadMap(const long* stream, Map& out)
{
    out.n = 0;
    long count = *stream++;
    for (; count > 0; --count) { long k = *stream++; long v = *stream++; out.insert(k, v); }
}
static int run()
{
    long stream[] = { 3, 1, 10, 2, 20, 1, 30 };
    Map m; m.n = 5; ReadMap(stream, m);
    return m.n == 2 && m.keys[0] == 1 && m.vals[0] == 10 && m.keys[1] == 2 && m.vals[1] == 20;
}
int main()
{
    int ok = run();
    printf(ok ? "00632830_TEST PASS\n" : "00632830_TEST FAIL\n");
    return ok ? 0 : 1;
}
