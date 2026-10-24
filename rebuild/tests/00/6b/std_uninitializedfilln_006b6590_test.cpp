// Behaviour test for 0x006B6590 std::uninitialized_fill_n(CThemeChannel*, unsigned int, const CThemeChannel&).
#include <cstdio>
#include <new>

static int g_copies = 0;

struct CThemeChannel {
    int m_id;
    unsigned char _rest[0x1f0 - 4];
    CThemeChannel() : m_id(0) { }
    CThemeChannel(const CThemeChannel& other) : m_id(other.m_id) { ++g_copies; }
};

struct FalseType { FalseType() { } };

template <class ForwardIter, class Size, class T>
ForwardIter UninitFillNAux(ForwardIter first, Size n, const T& x, const FalseType&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter UninitFillN(ForwardIter first, Size n, const T& x)
{
    return UninitFillNAux(first, n, x, FalseType());
}

static unsigned char raw[sizeof(CThemeChannel) * 4];

int main()
{
    int ok = sizeof(CThemeChannel) == 0x1f0;
    CThemeChannel* p = (CThemeChannel*)raw;
    CThemeChannel x; x.m_id = 42;
    CThemeChannel* end = UninitFillN(p, 3u, x);
    if (end != p + 3 || g_copies != 3 || p[0].m_id != 42 || p[1].m_id != 42 || p[2].m_id != 42) ok = 0;
    if (UninitFillN(p, 0u, x) != p || g_copies != 3) ok = 0;   // n == 0: nothing built, first returned
    if (!ok) { std::printf("006b6590_TEST FAIL\n"); return 1; }
    std::printf("006b6590_TEST PASS\n");
    return 0;
}