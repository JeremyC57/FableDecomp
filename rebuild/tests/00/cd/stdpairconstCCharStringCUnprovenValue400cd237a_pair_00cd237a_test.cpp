// behaviour test for 0x00CD237A std::pair<const CCharString, CUnprovenValue4_00cd237a>::pair(const pair&): the first member goes through its own copy (counted here), the 4-byte second member is copied verbatim, and the result is the destination
#include <cstdio>
static int g_copies = 0;
struct First { int v; First() : v(0) { } First(const First& o) : v(o.v) { ++g_copies; } First& operator=(const First& o) { v = o.v; ++g_copies; return *this; } };
struct Rec { First first; unsigned int second; };
static Rec* CopyMember(Rec* self, const Rec& o, bool construct)   // the function under test (construct or assign)
{
    if (construct) { First made(o.first); self->first.v = made.v; } else self->first = o.first;
    self->second = o.second;
    return self;
}
int main()
{
    int fail = 0;
    Rec a; a.first.v = 7; a.second = 0x33FFCC5Bu;
    Rec b; b.first.v = 1; b.second = 2;
    g_copies = 0;
    if (CopyMember(&b, a, true) != &b) fail |= 1;
    if (b.first.v != 7 || b.second != 0x33FFCC5Bu || g_copies != 1) fail |= 2;
    Rec c; c.first.v = 3; c.second = 4;
    if (CopyMember(&c, b, false) != &c || c.first.v != 7 || c.second != 0x33FFCC5Bu || g_copies != 2) fail |= 4;
    if (fail) { std::printf("00cd237a_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cd237a_TEST PASS\n");
    return 0;
}