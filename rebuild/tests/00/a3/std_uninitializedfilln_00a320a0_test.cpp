// behaviour test for 0x00A320A0 std::__uninitialized_fill_n over pair<unsigned long, C3DDummyObject> (model copy; record contents checked word by word)
#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define long int   // keep the retail 32-bit layout (LP64 hosts) so the byte comparison sees no padding
#endif
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject {                                   // Ego_r.pdb, 0x34 bytes
public:
    CMatrix3x4 Transform;                                // +0x00
    long BoneIndex;                                      // +0x30
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
typedef std::pair<unsigned long, C3DDummyObject> _Elem;
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
    if (!ok) { std::printf("00a320a0_TEST FAIL\n"); return 1; }
    std::printf("00a320a0_TEST PASS\n");
    return 0;
}