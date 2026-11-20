#include <cstdio>

// The row (0x00496EB0, the GFWrapLimit<long,int,int> instance) is linked in: declaration only here.
template <class T, class TMin, class TMax> void __fastcall GFWrapLimit(T& value, const TMin& minimum, const TMax& maximum);

static bool Check(long initial, int minimum, int maximum, long expected)
{
    long value = initial;
    GFWrapLimit<long, int, int>(value, minimum, maximum);
    return value == expected;
}

int main()
{
    if (!Check(-6, -5, 8, 8) ||
        !Check(-5, -5, 8, -5) ||
        !Check(2, -5, 8, 2) ||
        !Check(8, -5, 8, 8) ||
        !Check(9, -5, 8, -5))
    {
        std::printf("global_GFWrapLimit_00496eb0_test FAIL\n");
        return 1;
    }

    std::printf("global_GFWrapLimit_00496eb0_test PASS\n");
    return 0;
}
