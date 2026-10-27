// behaviour test for 0x0081A2B0 std::__uninitialized_fill_n over pair<EHeroMorphType, CParticleMorphs::CEntry> (model copy; record contents checked word by word)
#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define long int   // keep the retail 32-bit layout (LP64 hosts) so the byte comparison sees no padding
#endif
enum EHeroMorphType { EHeroMorphType_FULL_RANGE = 0x7FFFFFFF };   // full 32-bit range for the byte test
enum EMeshLocationPoint { EMeshLocationPoint_FULL_RANGE = 0x7FFFFFFF };   // full 32-bit range for the byte test
class CDefString {                                       // 4 bytes, an index into the def string table
public:
    long TablePos;
};
class CParticleMorphs {
public:
    class CEntry {                                       // Ego_r.pdb, 36 bytes, trivially copyable
    public:
        EHeroMorphType MorphType;                        // +0x00
        bool Blending;                                   // +0x04
        EMeshLocationPoint Location;                     // +0x08
        CDefString LocationName;                         // +0x0C
        long LocationIndex;                              // +0x10
        long ParticleIndex;                              // +0x14
        long SecondParticleIndex;                        // +0x18
        float StartAtMorphStrength;                      // +0x1C
        float EndAtMorphStrength;                        // +0x20
    };
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
}
typedef std::pair<EHeroMorphType, CParticleMorphs::CEntry> _Elem;
int main()
{
    static double xraw[(sizeof(_Elem) + 7) / 8], raw[4 * ((sizeof(_Elem) + 7) / 8)];
    unsigned char* xb = (unsigned char*)xraw;
    for (unsigned i = 0; i < sizeof(_Elem); ++i) xb[i] = (unsigned char)(i * 7 + 1);
    const _Elem& x = *(const _Elem*)xraw;
    std::memset(raw, 0, sizeof(raw));
    _Elem* first = (_Elem*)raw;
    _Elem* end = std::__uninitialized_fill_n(first, 3u, x, std::__false_type());
    int ok = end == first + 3 && std::memcmp(&first[2], xraw, sizeof(_Elem)) == 0 && std::memcmp(&first[0], xraw, sizeof(_Elem)) == 0;
    unsigned char* tail = (unsigned char*)(first + 3);
    for (unsigned i = 0; i < sizeof(_Elem); ++i) ok = ok && tail[i] == 0;
    ok = ok && std::__uninitialized_fill_n(first, 0u, x, std::__false_type()) == first;
    if (!ok) { std::printf("0081a2b0_TEST FAIL\n"); return 1; }
    std::printf("0081a2b0_TEST PASS\n");
    return 0;
}