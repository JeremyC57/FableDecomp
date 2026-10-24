// Behaviour test for 0x0067E7F0 std::__unguarded_insertion_sort_aux<CDegradableInfo*, CDegradableInfo,
// CDegradableDef::CIsHealth1GTHealth2>: with a larger-health guard in front, [first, last) ends up sorted by descending
// HealthPercentage, each stage moved whole (equal keys keep their order).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#pragma pack(push, 1)
class CEngineGraphic {
public:
    unsigned int  BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
};
#pragma pack(pop)

class CDegradableInfo {
public:
    float          HealthPercentage;
    CEngineGraphic EngineGraphic;
    int            SmashParticleEmitter;
    bool           BlocksNavigation;
};

class CDegradableDef {
public:
    class CIsHealth1GTHealth2 {
    public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const
        {
            return a.HealthPercentage > b.HealthPercentage;
        }
    };
};

static void Set(CDegradableInfo& d, float health, int tag)
{
    d.HealthPercentage = health; d.EngineGraphic.BankIndex = tag; d.EngineGraphic.AnimStep = 0.0f;
    d.EngineGraphic.RenderSizeX = 0.0f; d.EngineGraphic.AdditiveAlpha = false; d.EngineGraphic.Type = 0;
    d.SmashParticleEmitter = tag * 2; d.BlocksNavigation = (tag & 1) != 0;
}
static bool Is(const CDegradableInfo& d, float health, int tag)
{
    return d.HealthPercentage == health && (int)d.EngineGraphic.BankIndex == tag && d.SmashParticleEmitter == tag * 2 &&
           d.BlocksNavigation == ((tag & 1) != 0);
}

namespace nav {
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

int main()
{
    CDegradableInfo a[6];
    Set(a[0], 1.0f, 9);                                     // guard
    Set(a[1], 0.25f, 1); Set(a[2], 0.75f, 2); Set(a[3], 0.5f, 3); Set(a[4], 0.75f, 4); Set(a[5], 0.0f, 5);
    nav::__unguarded_insertion_sort_aux(a + 1, a + 6, (CDegradableInfo*)0, CDegradableDef::CIsHealth1GTHealth2());
    if (!Is(a[0], 1.0f, 9) || !Is(a[1], 0.75f, 2) || !Is(a[2], 0.75f, 4) || !Is(a[3], 0.5f, 3) || !Is(a[4], 0.25f, 1) ||
        !Is(a[5], 0.0f, 5)) {
        std::printf("0067e7f0_TEST FAIL\n"); return 1;
    }
    std::printf("0067e7f0_TEST PASS\n");
    return 0;
}