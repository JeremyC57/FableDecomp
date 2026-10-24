#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class C3DVector {
public:
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : x(v.x), y(v.y), z(v.z) {}
};
class CGetBestIciclePointForTarget {
public:
    C3DVector m_target;
    float m_bestScore;
    C3DVector m_bestPoint;
    CGetBestIciclePointForTarget() {}
    void operator()(const C3DVector& p)
    {
        float dx = p.x - m_target.x;
        float dy = p.y - m_target.y;
        float score = 1.0f / (dy * dy + dx * dx + 1.0f);
        if (score > m_bestScore) {
            m_bestScore = score;
            m_bestPoint = p;
        }
    }
};
int main()
{
    C3DVector pts[3];
    pts[0].x = 10; pts[0].y = 0; pts[0].z = 1;
    pts[1].x = 1;  pts[1].y = 1; pts[1].z = 2;     // closest to (0,0): d^2 = 2, score 1/3
    pts[2].x = 0;  pts[2].y = 5; pts[2].z = 3;
    CGetBestIciclePointForTarget f;
    f.m_target.x = 0; f.m_target.y = 0; f.m_target.z = 0; f.m_bestScore = 0; f.m_bestPoint = pts[0];
    CGetBestIciclePointForTarget r = std::for_each(pts, pts + 3, f);
    if (r.m_bestPoint.z != 2 || r.m_bestScore < 0.33f || r.m_bestScore > 0.34f) { std::printf("008e2f10_TEST FAIL\n"); return 1; }
    std::printf("008e2f10_TEST PASS\n");
    return 0;
}