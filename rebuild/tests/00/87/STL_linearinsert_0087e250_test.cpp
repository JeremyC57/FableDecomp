#include <cstdio>
#include <cstring>
#include <cmath>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of the STLport __linear_insert @ 0x0087E250: a value that sorts before the first entry moves the whole range up
// one slot (memmove copy_backward) and goes first; otherwise the unguarded insert finds its place.
struct CAnimationEntry { float Rotation; };
class CCloserTo {
public:
    float RotationAngle;
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        return fabs(RotationAngle - a->Rotation) < fabs(RotationAngle - b->Rotation);
    }
};
inline void* CopyTrivialBackward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (int)((const char*)__last - (const char*)__first);
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}
template <class _Tp>
inline _Tp** CopyBackward(_Tp** __first, _Tp** __last, _Tp** __result)
{
    return (_Tp**)CopyTrivialBackward(__first, __last, __result);
}
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall LinearInsert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        CopyBackward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        UnguardedLinearInsert(__last, __val, __comp);
}
int main()
{
    bool ok = true;
    CAnimationEntry e[5] = { {0.55f}, {0.65f}, {0.80f}, {0.51f}, {0.70f} };
    CCloserTo closer;
    closer.RotationAngle = 0.5f;
    const CAnimationEntry* v[5] = { &e[0], &e[1], &e[2], &e[3], 0 };
    LinearInsert(v, v + 3, v[3], closer);            // 0.51 is the new closest: shift everything
    if (v[0] != &e[3] || v[1] != &e[0] || v[2] != &e[1] || v[3] != &e[2]) ok = false;
    v[4] = &e[4];
    LinearInsert(v, v + 4, v[4], closer);            // 0.70 lands between 0.65 and 0.80
    if (v[0] != &e[3] || v[1] != &e[0] || v[2] != &e[1] || v[3] != &e[4] || v[4] != &e[2]) ok = false;
    if (!ok) { std::printf("0087e250_TEST FAIL\n"); return 1; }
    std::printf("0087e250_TEST PASS\n");
    return 0;
}