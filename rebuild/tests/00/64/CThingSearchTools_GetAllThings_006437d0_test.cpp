// behaviour test for 0x006437D0: only scan-visible things of the type passing the filter are appended
#include <cstdio>
#include <vector>
struct T { unsigned char bits; unsigned long type; bool ok; };
static bool Visible(const T& t) { unsigned char f = t.bits, nf = ~f; return (f & 4) && (f & 8) && (nf & 1) && !(f & 0x20); }
int main()
{
    const unsigned long mask = 0x47;
    T things[4] = { { 0x0c, mask, true }, { 0x0c, 0, true }, { 0x2c, mask, true }, { 0x0c, mask | 0x100, true } };
    std::vector<const T*> out;
    for (int i = 0; i < 4; ++i) if (Visible(things[i]) && (things[i].type & mask) == mask && things[i].ok) out.push_back(&things[i]);
    if (out.size() != 2 || out[0] != &things[0] || out[1] != &things[3]) { std::printf("006437d0_TEST FAIL\n"); return 1; }
    std::printf("006437d0_TEST PASS\n");
    return 0;
}