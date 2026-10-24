// Behaviour test for 0x00769EA0 CopyBackward_PairULongUpdateParamDesc_00769ea0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParamCallback;

template <class A, class B> class CParticleUpdateParamDesc {      // Ego_r.pdb, 12 bytes
public:
    virtual void v00();                                         // +0x00 vtable (not copied by the implicit operator=)
    CParticleUpdateParamCallback<A, B>* Callback;               // +0x04
    unsigned long                       Index;                  // +0x08
};
}

struct PairULongUpdateParamDesc {
    unsigned long first;                                                            // +0x00
    NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> second; // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairULongUpdateParamDesc* __fastcall CopyBackward_PairULongUpdateParamDesc_00769ea0(PairULongUpdateParamDesc* first, PairULongUpdateParamDesc* last, PairULongUpdateParamDesc* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template <class A, class B> void NParticleEngine::CParticleUpdateParamDesc<A, B>::v00() { }

typedef PairULongUpdateParamDesc T;
#define KEY(E) (E).first
#define INIT(E) (E).second.Index = 11; (E).second.Callback = 0
#define CALL(f, l, r) CopyBackward_PairULongUpdateParamDesc_00769ea0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(a[3].second.Index == 11);
    if (g_fail) { std::printf("00769ea0_TEST FAIL\n"); return 1; }
    std::printf("00769ea0_TEST PASS\n");
    return 0;
}