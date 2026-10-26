#include <cstdio>
#include <cstring>
#include <cstdlib>

class CThing { public: unsigned char pad[0x18]; unsigned int uidLo; unsigned int uidHi; };
struct CFilter { unsigned int lo, hi; CFilter(const CThing* t) : lo(t->uidLo), hi(t->uidHi) { } };
int main()
{
    CThing t; std::memset(&t, 0, sizeof t); t.uidLo = 0x12345678; t.uidHi = 0x9abcdef0;
    CFilter f(&t);
    if (f.lo != 0x12345678 || f.hi != 0x9abcdef0) { std::printf("006af290_TEST FAIL\n"); return 1; }
    if (sizeof(f) != 8) { std::printf("006af290_TEST FAIL\n"); return 1; }
    std::printf("006af290_TEST PASS\n");
    return 0;
}