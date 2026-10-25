// Behaviour test for 0x0040DDC0 std::__uninitialized_copy<CDisplayModeDesc*>: copies every descriptor into raw storage
// through the copy constructor and returns the end of the copied range; an empty range copies nothing.
#include <cstdio>
#include <new>
class C2DExtentsI { public: long X; long Y; };
class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;
    long BitsPerPixel;
    long RefreshRate;
    CDisplayModeDesc() { Dimensions.X = 0; Dimensions.Y = 0; BitsPerPixel = 0; RefreshRate = 0; }
    CDisplayModeDesc(const CDisplayModeDesc& o) : Dimensions(o.Dimensions), BitsPerPixel(o.BitsPerPixel), RefreshRate(o.RefreshRate) { }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}
int main()
{
    CDisplayModeDesc src[3];
    for (int i = 0; i < 3; ++i) { src[i].Dimensions.X = 640 + i; src[i].Dimensions.Y = 480 + i; src[i].BitsPerPixel = 32; src[i].RefreshRate = 60 + i; }
    unsigned char raw[3 * sizeof(CDisplayModeDesc)];
    CDisplayModeDesc* dst = (CDisplayModeDesc*)raw;
    CDisplayModeDesc* end = std::__uninitialized_copy(src, src + 3, dst, std::__false_type());
    if (sizeof(CDisplayModeDesc) != 16 || end != dst + 3 || dst[2].Dimensions.X != 642 || dst[1].RefreshRate != 61 || dst[0].BitsPerPixel != 32) {
        std::printf("0040ddc0_TEST FAIL\n"); return 1;
    }
    if (std::__uninitialized_copy(src, src, dst, std::__false_type()) != dst) { std::printf("0040ddc0_TEST FAIL\n"); return 1; }
    std::printf("0040ddc0_TEST PASS\n");
    return 0;
}
