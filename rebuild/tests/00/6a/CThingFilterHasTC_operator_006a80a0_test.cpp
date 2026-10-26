// Behaviour test for 006a80a0: CThingFilter_HasTC over a bitset<274> component mask (model copy; in-range positions only).
#include <cstdio>
#include <cstdlib>
template <unsigned int _Nb> class bitset {
public:
    unsigned int _M_w[(_Nb + 31) / 32];
    bool test(unsigned int pos) const
    {
        if (pos >= _Nb) { std::puts("bitset"); std::abort(); }
        return (_M_w[pos / 32] & (1u << (pos % 32))) != 0;
    }
};
class CThing { public: bitset<274> UsingComponents; };
class CThingFilter_HasTC { public:
    int Type;
    bool operator()(const CThing& thing) const { return thing.UsingComponents.test(Type); }
};
int main()
{
    CThing t; int fail = 0;
    for (int i = 0; i < 9; ++i) t.UsingComponents._M_w[i] = 0;
    t.UsingComponents._M_w[0] = 1u << 7;        // creature navigation (7)
    t.UsingComponents._M_w[2] = 1u << 6;        // carrying (0x46)
    t.UsingComponents._M_w[8] = 1u << 17;       // the last valid bit (273)
    int on[3] = {7, 0x46, 273};
    for (int i = 0; i < 3; ++i) { CThingFilter_HasTC f; f.Type = on[i]; if (!f(t)) fail = 1; }
    int off[4] = {0, 6, 0x48, 272};
    for (int i = 0; i < 4; ++i) { CThingFilter_HasTC f; f.Type = off[i]; if (f(t)) fail = 1; }
    if (fail) { std::printf("006a80a0_TEST FAIL\n"); return 1; }
    std::printf("006a80a0_TEST PASS\n");
    return 0;
}