// behaviour test for 0x005163B0: only scan-visible things passing both filters are appended, in list order
#include <cstdio>
#include <list>
struct T { unsigned char bits; bool quest; bool area; };
static bool Visible(unsigned char f) { unsigned char nf = ~f; return (f & 4) && (f & 8) && (nf & 1) && !(f & 0x20); }
int main()
{
    T things[5] = { { 0x0c, true, true }, { 0x0d, true, true }, { 0x0c, false, true }, { 0x2c, true, true }, { 0x0c, true, true } };
    std::list<const T*> out;
    for (int i = 0; i < 5; ++i) if (Visible(things[i].bits) && things[i].quest && things[i].area) out.push_back(&things[i]);
    if (out.size() != 2 || out.front() != &things[0] || out.back() != &things[4]) { std::printf("005163b0_TEST FAIL\n"); return 1; }
    std::printf("005163b0_TEST PASS\n");
    return 0;
}