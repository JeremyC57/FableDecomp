// behaviour test for 0x00617A00: only scan-visible things passing both filters are appended, in list order
#include <cstdio>
#include <list>
struct T { unsigned char bits; bool f1; bool f2; };
static bool Visible(const T& t) { unsigned char f = t.bits, nf = ~f; return (f & 4) && (f & 8) && (nf & 1) && !(f & 0x20); }
int main()
{
    T things[4] = { { 0x0c, true, true }, { 0x0d, true, true }, { 0x0c, false, true }, { 0x0c, true, true } };
    std::list<const T*> out;
    for (int i = 0; i < 4; ++i) if (Visible(things[i]) && things[i].f1 && things[i].f2) out.push_back(&things[i]);
    if (out.size() != 2 || out.front() != &things[0] || out.back() != &things[3]) { std::printf("00617a00_TEST FAIL\n"); return 1; }
    std::printf("00617a00_TEST PASS\n");
    return 0;
}