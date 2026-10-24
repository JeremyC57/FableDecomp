// Behaviour test for 0x0085D680 std::fill<CTextureManagerPoolInfo*, CTextureManagerPoolInfo> (chunk 402): every element
// of [first, last) is assigned the value, nothing outside the range is touched.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

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
        : Width(other.Width), Height(other.Height), PixelFormat(other.PixelFormat), UseMipmaps(other.UseMipmaps), Count(other.Count) {}
    bool Same(const CTextureManagerPoolInfo& o) const
    {
        return Width == o.Width && Height == o.Height && PixelFormat.FormatIndex == o.PixelFormat.FormatIndex
            && UseMipmaps == o.UseMipmaps && Count == o.Count;
    }
};
namespace stl402 {
template<class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
}

int main()
{
    int failures = 0;
    CTextureManagerPoolInfo pool[6];
    CTextureManagerPoolInfo value;
    value.Width = 256; value.Height = 128; value.PixelFormat.FormatIndex = 7; value.UseMipmaps = 1; value.Count = 3;
    stl402::fill(pool + 1, pool + 5, value);
    for (int i = 1; i < 5; ++i)
        if (!pool[i].Same(value)) { printf("  element %d not filled\n", i); ++failures; }
    if (pool[0].Width != 0 || pool[5].Width != 0) { printf("  wrote outside the range\n"); ++failures; }
    stl402::fill(pool + 5, pool + 5, value);
    if (pool[5].Width != 0) { printf("  empty range wrote\n"); ++failures; }
    if (failures == 0) {
        printf("0085d680_TEST PASS\n");
        return 0;
    }
    printf("0085d680_TEST FAIL\n");
    return 1;
}