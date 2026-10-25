// Behaviour test for 0x00B3E340 std::__uninitialized_copy<CTextureManagerPoolInfo*, CTextureManagerPoolInfo*>: placement
// copy of 20-byte texture-pool records into raw storage, returning the advanced destination.
#include <cstdio>
#include <new>
class CPixelFormat { public: long FormatIndex; CPixelFormat() : FormatIndex(-1) { } };
class CTextureManagerPoolInfo {
public:
    unsigned long Width, Height; CPixelFormat PixelFormat; bool UseMipmaps; unsigned long Count;
};
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first);
    return __cur;
}
int main()
{
    bool ok = true;
    CTextureManagerPoolInfo src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Width = 64u << i; src[i].Height = 32u << i; src[i].PixelFormat.FormatIndex = 7 + i;
        src[i].UseMipmaps = (i & 1) != 0; src[i].Count = 10 + i;
    }
    static double storage[64];
    CTextureManagerPoolInfo* dst = (CTextureManagerPoolInfo*)storage;
    CTextureManagerPoolInfo* r = __uninitialized_copy(src, src + 3, dst, __false_type());
    if (r != dst + 3 || sizeof(CTextureManagerPoolInfo) != 20) ok = false;
    for (int i = 0; i < 3; ++i)
        if (dst[i].Width != src[i].Width || dst[i].Height != src[i].Height || dst[i].PixelFormat.FormatIndex != src[i].PixelFormat.FormatIndex
            || dst[i].UseMipmaps != src[i].UseMipmaps || dst[i].Count != src[i].Count) ok = false;
    if (__uninitialized_copy(src, src, dst, __false_type()) != dst) ok = false;
    if (!ok) { std::printf("00b3e340_TEST FAIL\n"); return 1; }
    std::printf("00b3e340_TEST PASS\n");
    return 0;
}
