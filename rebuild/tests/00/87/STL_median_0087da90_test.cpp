#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of std::__median @ 0x0087DA90 (median of three under a comparator), with a sane comparator and the retail
// CTurningAnimCompare (which reads its left operand twice and so never answers true).
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
struct CIntLess {
    bool operator()(int a, int b) const { return a < b; }
};
template <class _Tp, class _Compare>
const _Tp& __fastcall Median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
int main()
{
    bool ok = true;
    int p[6][3] = { {1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1} };
    for (int i = 0; i < 6; ++i)
        if (Median(p[i][0], p[i][1], p[i][2], CIntLess()) != 2) ok = false;
    CAnimationEntry e0 = { 0.2f }, e1 = { 0.7f }, e2 = { 0.4f };
    const CAnimationEntry* a = &e0;
    const CAnimationEntry* b = &e1;
    const CAnimationEntry* c = &e2;
    CTurningAnimCompare comp;
    comp.RotationAngle = 0.5f;
    if (&Median(a, b, c, comp) != &b) ok = false;    // never-true comparator: the middle argument
    if (!ok) { std::printf("0087da90_TEST FAIL\n"); return 1; }
    std::printf("0087da90_TEST PASS\n");
    return 0;
}