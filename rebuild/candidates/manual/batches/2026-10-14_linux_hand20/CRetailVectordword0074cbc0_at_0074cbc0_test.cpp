#include <cstdio>
static int g_aborts = 0;
struct V {
    unsigned long* first; unsigned long* last; unsigned long* e; unsigned long size() const { return (unsigned long)(last - first); }
    unsigned long& at(unsigned long n) { if (n >= size()) { ++g_aborts; return first[0]; } return first[n]; }
};
int main()
{
    unsigned long d[3] = { 10, 20, 30 }; V v; v.first = d; v.last = d + 3;
    if (&v.at(2) != &d[2] || v.at(0) != 10 || g_aborts != 0) { std::printf("0074cbc0_TEST FAIL\n"); return 1; }
    v.at(3); if (g_aborts != 1) { std::printf("0074cbc0_TEST FAIL\n"); return 1; }
    std::printf("0074cbc0_TEST PASS\n");
    return 0;
}
