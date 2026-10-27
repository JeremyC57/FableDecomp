// Behaviour test for 0x007A6B20: ReplaceMeshWithNext advances the mesh index, wrapping at the def's mesh count
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SMesh { unsigned long MeshIndex; unsigned long Count; unsigned long Shown;
    void ReplaceMeshByIndex(unsigned long i) { Shown = i; }
    void ReplaceMeshWithNext() { if (++MeshIndex >= Count) MeshIndex = 0; ReplaceMeshByIndex(MeshIndex); } };
int main()
{
    SMesh m; m.MeshIndex = 0; m.Count = 3; m.Shown = 99;
    m.ReplaceMeshWithNext(); CHECK(m.MeshIndex == 1 && m.Shown == 1);
    m.ReplaceMeshWithNext(); CHECK(m.Shown == 2);
    m.ReplaceMeshWithNext(); CHECK(m.MeshIndex == 0 && m.Shown == 0);
    if (!g_fail) printf("007a6b20_TEST PASS\n");
    return g_fail;
}