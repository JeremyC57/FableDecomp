// behaviour test for 0x004B8190: every source record is copy-constructed in order and the end of the run is returned
#include <cstdio>
#include <new>
#include <string>
struct E { std::string s; long a; bool b; long c; };
static E* Copy(const E* f, const E* l, E* r) { for (; f != l; ++f, ++r) new (r) E(*f); return r; }
int main()
{
    E src[2]; src[0].s = "x"; src[0].a = 1; src[0].b = true; src[0].c = 5; src[1].s = "yy"; src[1].a = 2; src[1].b = false; src[1].c = 6;
    double raw[(sizeof(E) * 2 + 7) / 8]; E* dst = (E*)raw;
    E* end = Copy(src, src + 2, dst);
    int ok = end == dst + 2 && dst[0].s == "x" && dst[1].s == "yy" && dst[1].c == 6 && dst[0].b;
    dst[0].~E(); dst[1].~E();
    if (!ok) { std::printf("004b8190_TEST FAIL\n"); return 1; }
    std::printf("004b8190_TEST PASS\n");
    return 0;
}