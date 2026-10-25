#include <cstdio>
// Behaviour test for 0x009322D0 std::__median<CThing*, bool (__fastcall*)(CThing*, CThing*)>: links against the source
// object's instantiation; every permutation of three keyed things returns a reference to the median element.
class CThing { public: int Z; };
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);
namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);
}
static int g_compares = 0;
static bool __fastcall Lower(CThing* a, CThing* b) { ++g_compares; return a->Z < b->Z; }
int main()
{
    CThing items[3]; CThing* x[3];
    int perms[6][3] = { {1,2,3}, {1,3,2}, {2,1,3}, {2,3,1}, {3,1,2}, {3,2,1} };
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) { items[j].Z = perms[i][j]; x[j] = &items[j]; }
        CThing* const& m = std::__median<CThing*, ThingCompare>(x[0], x[1], x[2], &Lower);
        if (m->Z != 2 || (&m != &x[0] && &m != &x[1] && &m != &x[2])) { std::printf("009322d0_TEST FAIL perm %d\n", i); return 1; }
    }
    if (g_compares < 12) { std::printf("009322d0_TEST FAIL compares\n"); return 1; }
    std::printf("009322d0_TEST PASS\n");
    return 0;
}
