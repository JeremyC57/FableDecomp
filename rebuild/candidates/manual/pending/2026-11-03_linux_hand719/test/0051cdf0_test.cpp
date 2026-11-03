#include <cstdio>
// Model of ??_ECWorldMap@@W3AEPAXI@Z: the adjustor thunk for the CIDrawWorldMap base at +4 moves `this` back by 4
// and runs the primary vector deleting destructor, so deleting through the secondary base destroys the whole object.
static int g_dtor;
struct Base0 { virtual ~Base0() { } };
struct IDraw { virtual ~IDraw() { } };
struct WorldMap : Base0, IDraw { int marker; WorldMap() : marker(0x1234) { } virtual ~WorldMap() { if (marker == 0x1234) ++g_dtor; } };
int main()
{
    WorldMap* m = new WorldMap;
    IDraw* d = m;
    if ((char*)d - (char*)m != 4) { std::printf("0051cdf0_TEST FAIL\n"); return 1; }
    delete d;
    if (g_dtor != 1) { std::printf("0051cdf0_TEST FAIL\n"); return 1; }
    std::printf("0051cdf0_TEST PASS\n"); return 0;
}
