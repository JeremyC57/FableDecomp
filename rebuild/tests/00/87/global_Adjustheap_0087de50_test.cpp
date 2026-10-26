// behaviour test for 0087de50: _Adjust_heap over animation-entry pointers with the shipped
// CTurningAnimCompare, which reads its FIRST argument's rotation twice -- the comparison is always
// false, so every sift step takes the right-hand child (idx is never decremented).
#include <cstdio>
#include <cmath>
class CAnimationEntry { public:
    float Rotation;
    void GetRotationInAnimation(float* pRotation) const { *pRotation = -Rotation; }
};
struct CTurningAnimCompare {
    float m_WantedRotation;
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        float rotationA;
        float rotationB;
        a->GetRotationInAnimation(&rotationA);
        a->GetRotationInAnimation(&rotationB);
        return std::fabs(m_WantedRotation - rotationA) < std::fabs(m_WantedRotation - rotationB);
    }
};
typedef const CAnimationEntry* P;
static int g_pushTop = -1;
static void PushHeap(P* first, long hole, long top, P value, CTurningAnimCompare pred)
{
    g_pushTop = (int)top;
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(P* first, long hole, long bottom, P value, CTurningAnimCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeap(first, hole, top, value, pred);
}
int main()
{
    CAnimationEntry e[7];
    P a[7];
    long i = 0;
    for (i = 0; i < 7; ++i) { e[i].Rotation = (float)(i * 10); a[i] = &e[i]; }
    CTurningAnimCompare pred;
    pred.m_WantedRotation = -15.0f;
    int ok = 1;
    if (pred(&e[1], &e[2]) || pred(&e[2], &e[1])) ok = 0;
    CAnimationEntry extra;
    extra.Rotation = 99.0f;
    // hole 0, bottom 7: right children 2 then 6 move up, the value lands in slot 6
    AdjustHeap(a, 0, 7, &extra, pred);
    if (a[0] != &e[2] || a[2] != &e[6] || a[6] != &extra || g_pushTop != 0) ok = 0;
    if (a[1] != &e[1] || a[3] != &e[3] || a[4] != &e[4] || a[5] != &e[5]) ok = 0;
    // bottom 6 from hole 1: idx 4 < 6 -> slot 1 takes a[4], idx 10 stops; value lands at 4
    AdjustHeap(a, 1, 6, &e[0], pred);
    if (a[1] != &e[4] || a[4] != &e[0]) ok = 0;
    // even bottom: hole 0, bottom 2 -> only child at the bottom (idx == bottom) takes slot 0
    P b[2];
    b[0] = &e[0]; b[1] = &e[1];
    AdjustHeap(b, 0, 2, &e[3], pred);
    if (b[0] != &e[1] || b[1] != &e[3]) ok = 0;
    if (!ok) { std::printf("0087de50_TEST FAIL\n"); return 1; }
    std::printf("0087de50_TEST PASS\n");
    return 0;
}