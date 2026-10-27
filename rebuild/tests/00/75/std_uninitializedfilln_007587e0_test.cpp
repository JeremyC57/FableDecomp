// behaviour test for 0x007587E0 std::uninitialized_fill_n over pair<EBodyLocation, CWoundMorphs::CEntry> (model copy; record contents checked word by word)
#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define long int   // keep the retail 32-bit layout (LP64 hosts) so the byte comparison sees no padding
#endif
enum EBodyLocation { EBodyLocation_FULL_RANGE = 0x7FFFFFFF };   // full 32-bit range for the byte test                                  // Ego_r.pdb (enumerators not needed)
class CWoundMorphs {
public:
    class CEntry {                                       // 40 bytes (Ego_r.pdb), trivially copyable
    public:
        EBodyLocation Location;                          // +0x00
        long ReplacingTextureIndex;                      // +0x04
        long ReplacingBumpMapIndex;                      // +0x08
        long WoundTextureIndex;                          // +0x0C
        long WoundBumpMapIndex;                          // +0x10
        long ScarTextureIndex;                           // +0x14
        long ScarBumpMapIndex;                           // +0x18
        long FadeToScarDuration;                         // +0x1C
        long TotalDuration;                              // +0x20
        float PermanentScarStrength;                     // +0x24
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
typedef std::pair<EBodyLocation, CWoundMorphs::CEntry> _Elem;
int main()
{
    static double xraw[(sizeof(_Elem) + 7) / 8], raw[4 * ((sizeof(_Elem) + 7) / 8)];
    unsigned char* xb = (unsigned char*)xraw;
    for (unsigned i = 0; i < sizeof(_Elem); ++i) xb[i] = (unsigned char)(i * 7 + 1);
    const _Elem& x = *(const _Elem*)xraw;
    std::memset(raw, 0, sizeof(raw));
    _Elem* first = (_Elem*)raw;
    _Elem* end = std::uninitialized_fill_n(first, 3u, x);
    int ok = end == first + 3 && std::memcmp(&first[2], xraw, sizeof(_Elem)) == 0 && std::memcmp(&first[0], xraw, sizeof(_Elem)) == 0;
    unsigned char* tail = (unsigned char*)(first + 3);
    for (unsigned i = 0; i < sizeof(_Elem); ++i) ok = ok && tail[i] == 0;
    ok = ok && std::uninitialized_fill_n(first, 0u, x) == first;
    if (!ok) { std::printf("007587e0_TEST FAIL\n"); return 1; }
    std::printf("007587e0_TEST PASS\n");
    return 0;
}