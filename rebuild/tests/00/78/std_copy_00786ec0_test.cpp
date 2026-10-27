// behaviour test for 0x00786EC0: the copy assigns each element in order and returns the end of the destination
#include <cstdio>
#include <string>
struct E { long a; std::string s; long c; };
static E* Copy(const E* first, const E* last, E* result) { for (long n = (long)(last - first); n > 0; --n) { *result = *first; ++first; ++result; } return result; }
int main()
{
    E v[3]; v[0].a = 1; v[0].s = "x"; v[0].c = 0; v[1].a = 2; v[1].s = "yy"; v[1].c = 1; v[2].a = 3; v[2].s = "z"; v[2].c = 0;
    E* r = Copy(v + 1, v + 3, v);
    E* e = Copy(v, v, v + 2);
    if (r != v + 2 || e != v + 2 || v[0].a != 2 || v[0].s != "yy" || v[0].c != 1 || v[1].a != 3 || v[1].s != "z") { std::printf("00786ec0_TEST FAIL\n"); return 1; }
    std::printf("00786ec0_TEST PASS\n");
    return 0;
}