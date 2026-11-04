// Behaviour test for 0x009E3B60 (parked row): the row's logic over a flat model.
#include <cstdio>

static unsigned char Channel(unsigned long v, long shift, long bits) {
    if (!bits) return 0; unsigned long m = ((1ul << bits) - 1); unsigned long c = (v >> shift) & m; return (unsigned char)((c * 255) / m);
}
int main() {
    unsigned long v = (31u << 11) | (0u << 5) | 31u;   // R5 G6 B5: red + blue full, green zero
    bool ok = Channel(v, 0, 5) == 255 && Channel(v, 5, 6) == 0 && Channel(v, 11, 5) == 255 && Channel(v, 16, 0) == 0;
    ok = ok && Channel(0x8000u, 15, 1) == 255;
    std::printf(ok ? "009e3b60_TEST PASS\n" : "009e3b60_TEST FAIL\n");
    return ok ? 0 : 1;
}
