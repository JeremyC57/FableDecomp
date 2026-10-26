#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of the STLport __unguarded_linear_insert @ 0x0087DC30 over turning-animation pointers: with a working
// rotation-distance comparator it shifts the farther entries up; with the retail CTurningAnimCompare (never true)
// it just stores the value at `last`.
class CAnimationEntry { public:
    float Rotation;
    void GetRotationInAnimation(float* rotation) const { *rotation = Rotation; }
};
class CTurningAnimCompare {
public:
    float RotationAngle;
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        float rotation_a;
        float rotation_b;
        a->GetRotationInAnimation(&rotation_a);
        a->GetRotationInAnimation(&rotation_b);
        float delta_a = (float)fabs(RotationAngle - rotation_a);
        float delta_b = (float)fabs(RotationAngle - rotation_b);
        if (delta_a < delta_b)
            return true;
        return false;
    }
};
class CCloserTo {                                        // the comparator as intended (reads both entries)
public:
    float RotationAngle;
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        return fabs(RotationAngle - a->Rotation) < fabs(RotationAngle - b->Rotation);
    }
};
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall UnguardedLinearInsert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
int main()
{
    bool ok = true;
    CAnimationEntry e[5] = { {0.50f}, {0.55f}, {0.70f}, {0.90f}, {0.60f} };
    const CAnimationEntry* v[5] = { &e[0], &e[1], &e[2], &e[3], &e[4] };
    CCloserTo closer;
    closer.RotationAngle = 0.5f;
    UnguardedLinearInsert(v + 4, v[4], closer);      // 0.60 goes between 0.55 and 0.70
    if (v[0] != &e[0] || v[1] != &e[1] || v[2] != &e[4] || v[3] != &e[2] || v[4] != &e[3]) ok = false;
    const CAnimationEntry* w[3] = { &e[3], &e[2], &e[0] };
    CTurningAnimCompare quirk;
    quirk.RotationAngle = 0.5f;
    UnguardedLinearInsert(w + 2, &e[1], quirk);      // never true: no shift
    if (w[0] != &e[3] || w[1] != &e[2] || w[2] != &e[1]) ok = false;
    if (!ok) { std::printf("0087dc30_TEST FAIL\n"); return 1; }
    std::printf("0087dc30_TEST PASS\n");
    return 0;
}