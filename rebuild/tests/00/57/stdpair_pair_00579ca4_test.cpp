// behaviour test for 0x00579CA4 std::pair<EHeroAbility, SExpSpendUndo>::pair(const first_type&, const second_type&): first takes the 4-byte key verbatim, second is copy-constructed (counted here) from b, and the result is the object itself
#include <cstdio>
static int g_copies = 0;
struct Second { int v[3]; Second() { v[0] = v[1] = v[2] = 0; } Second(const Second& o) { v[0] = o.v[0]; v[1] = o.v[1]; v[2] = o.v[2]; ++g_copies; } };
struct Pair { unsigned int first; Second second; };
static Pair* Construct(Pair* self, const unsigned int& a, const Second& b)   // the function under test
{
    self->first = a;
    Second made(b);
    self->second = made;
    return self;
}
int main()
{
    int fail = 0;
    Second s; s.v[0] = 5; s.v[1] = 6; s.v[2] = 7;
    unsigned int key = 0x80000001u;
    Pair p; p.first = 0;
    g_copies = 0;
    if (Construct(&p, key, s) != &p) fail |= 1;
    if (p.first != 0x80000001u || p.second.v[0] != 5 || p.second.v[2] != 7 || g_copies != 1) fail |= 2;
    if (fail) { std::printf("00579ca4_TEST FAIL %d\n", fail); return 1; }
    std::printf("00579ca4_TEST PASS\n");
    return 0;
}