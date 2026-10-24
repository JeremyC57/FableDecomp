// Behaviour test for 0x007697C0 STLport __uninitialized_copy over ParticleAndEndPoints: every element of
// [first, last) is copy-constructed into raw storage (strings deep-copied), the end of the range is returned.
#include <cstdio>
#include <cstring>
#include <new>

static int gCopies = 0;

class CCharString {
public:
    char* PStringData;
    CCharString(const char* s) { PStringData = new char[std::strlen(s) + 1]; std::strcpy(PStringData, s); }
    CCharString(const CCharString& o) { ++gCopies; PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); }
    ~CCharString() { delete[] PStringData; }
};

struct ParticleAndEndPoints {
    CCharString StartDummy;
    CCharString EndDummy;
    long        ParticleType;
    long        ParticleUID;
    long        PeriodicFrames;
    ParticleAndEndPoints(const char* a, const char* b, long t, long u, long p)
        : StartDummy(a), EndDummy(b), ParticleType(t), ParticleUID(u), PeriodicFrames(p) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter uninitialized_copy_model(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

int main()
{
    ParticleAndEndPoints src[2] = { ParticleAndEndPoints("hand_l", "tip_l", 3, 10, 7), ParticleAndEndPoints("hand_r", "tip_r", 4, 11, 9) };
    double raw[16];
    ParticleAndEndPoints* dst = (ParticleAndEndPoints*)raw;
    const ParticleAndEndPoints* first = src;
    ParticleAndEndPoints* end = uninitialized_copy_model(first, first + 2, dst);
    int ok = (end == dst + 2) && gCopies == 4;
    if (std::strcmp(dst[0].StartDummy.PStringData, "hand_l") || std::strcmp(dst[1].EndDummy.PStringData, "tip_r")) ok = 0;
    if (dst[0].StartDummy.PStringData == src[0].StartDummy.PStringData) ok = 0;
    if (dst[0].ParticleType != 3 || dst[1].ParticleUID != 11 || dst[1].PeriodicFrames != 9) ok = 0;
    if (uninitialized_copy_model(first, first, dst) != dst) ok = 0;
    dst[0].~ParticleAndEndPoints(); dst[1].~ParticleAndEndPoints();
    if (!ok) { std::printf("007697c0_TEST FAIL\n"); return 1; }
    std::printf("007697c0_TEST PASS\n");
    return 0;
}