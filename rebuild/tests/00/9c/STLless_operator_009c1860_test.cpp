#include <cstdio>

class C2DExtentsI {
public:
    long X;
    long Y;
};

class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;
    long        BitsPerPixel;
    long        RefreshRate;
    bool operator<(const CDisplayModeDesc& other) const
    {
        if (Dimensions.X != other.Dimensions.X)
            return Dimensions.X < other.Dimensions.X;
        if (Dimensions.Y != other.Dimensions.Y)
            return Dimensions.Y < other.Dimensions.Y;
        if (BitsPerPixel != other.BitsPerPixel)
            return BitsPerPixel < other.BitsPerPixel;
        return RefreshRate < other.RefreshRate;
    }
};

namespace std {
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};
template <class _Tp> bool less<_Tp>::operator()(const _Tp& __x, const _Tp& __y) const
{
    return __x < __y;
}
}

static CDisplayModeDesc Mode(long x, long y, long bpp, long hz)
{
    CDisplayModeDesc d;
    d.Dimensions.X = x; d.Dimensions.Y = y; d.BitsPerPixel = bpp; d.RefreshRate = hz;
    return d;
}

int main()
{
    std::less<CDisplayModeDesc> less;
    struct { CDisplayModeDesc a, b; bool expect; } cases[] = {
        { Mode(640, 480, 32, 60), Mode(800, 600, 16, 50), true },     // width decides
        { Mode(800, 600, 16, 50), Mode(640, 480, 32, 60), false },
        { Mode(800, 480, 32, 60), Mode(800, 600, 16, 50), true },     // height decides
        { Mode(800, 600, 16, 85), Mode(800, 600, 32, 60), true },     // depth decides
        { Mode(800, 600, 32, 60), Mode(800, 600, 32, 75), true },     // refresh decides
        { Mode(800, 600, 32, 75), Mode(800, 600, 32, 60), false },
        { Mode(800, 600, 32, 60), Mode(800, 600, 32, 60), false },    // equal is not less
        { Mode(-1, 600, 32, 60), Mode(0, 600, 32, 60), true },        // signed compare
    };
    for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
        if (less(cases[i].a, cases[i].b) != cases[i].expect) { std::printf("009c1860_TEST FAIL\n"); return 1; }
    std::printf("009c1860_TEST PASS\n");
    return 0;
}