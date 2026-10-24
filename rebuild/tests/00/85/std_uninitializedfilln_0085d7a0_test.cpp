// Behaviour test for 0x0085D7A0 std::__uninitialized_fill_n<CTextureManagerPoolInfo*, unsigned int, ...> (chunk 402): n
// copies are constructed in raw storage through the copy constructor and the end of the filled range is returned.
#include <cstdio>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static int gCopies = 0;
class CPixelFormat {
public:
    long FormatIndex;
};
class CTextureManagerPoolInfo {
public:
    unsigned long Width;
    unsigned long Height;
    CPixelFormat  PixelFormat;
    unsigned long UseMipmaps;
    unsigned long Count;
    CTextureManagerPoolInfo() : Width(0), Height(0), UseMipmaps(0), Count(0) { PixelFormat.FormatIndex = 0; }
    CTextureManagerPoolInfo(const CTextureManagerPoolInfo& other)
        : Width(other.Width), Height(other.Height), PixelFormat(other.PixelFormat), UseMipmaps(other.UseMipmaps), Count(other.Count)
    {
        ++gCopies;
    }
};
namespace stl402 {
struct __false_type {};
template<class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template<class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

int main()
{
    int failures = 0;
    union { double align; unsigned char bytes[sizeof(CTextureManagerPoolInfo) * 4]; } raw;
    for (unsigned int i = 0; i < sizeof(raw.bytes); ++i)
        raw.bytes[i] = 0xAB;
    CTextureManagerPoolInfo* first = (CTextureManagerPoolInfo*)raw.bytes;
    CTextureManagerPoolInfo value;
    value.Width = 64; value.Height = 32; value.PixelFormat.FormatIndex = 3; value.UseMipmaps = 1; value.Count = 9;
    stl402::__false_type tag;
    CTextureManagerPoolInfo* end = stl402::__uninitialized_fill_n(first, 3u, value, tag);
    if (end != first + 3) { printf("  wrong end\n"); ++failures; }
    if (gCopies != 3) { printf("  %d copies constructed\n", gCopies); ++failures; }
    for (int i = 0; i < 3; ++i)
        if (first[i].Width != 64 || first[i].Height != 32 || first[i].PixelFormat.FormatIndex != 3 || first[i].UseMipmaps != 1
            || first[i].Count != 9) { printf("  element %d wrong\n", i); ++failures; }
    if (raw.bytes[sizeof(CTextureManagerPoolInfo) * 3] != 0xAB) { printf("  wrote past n\n"); ++failures; }
    if (stl402::__uninitialized_fill_n(first, 0u, value, tag) != first) { printf("  n = 0 moved\n"); ++failures; }
    if (failures == 0) {
        printf("0085d7a0_TEST PASS\n");
        return 0;
    }
    printf("0085d7a0_TEST FAIL\n");
    return 1;
}