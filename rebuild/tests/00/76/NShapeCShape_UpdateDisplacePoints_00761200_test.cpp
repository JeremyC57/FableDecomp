#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
};
struct CShape {
    CRetailVector<C3DVector*> SelectedPoints;
    bool UpdateDisplacePoints(const C3DVector& delta)
    {
        bool moved = false;
        for (C3DVector** it = SelectedPoints.begin(); it != SelectedPoints.end(); ++it) { moved = true; **it += delta; }
        return moved;
    }
};
int main()
{
    C3DVector pts[3]; for (int i = 0; i < 3; ++i) { pts[i].X = (float)i; pts[i].Y = 10.0f; pts[i].Z = -1.0f; }
    C3DVector* sel[2] = { &pts[0], &pts[2] };
    CShape s; s.SelectedPoints.m_pFirst = sel; s.SelectedPoints.m_pLast = sel + 2; s.SelectedPoints.m_pEnd = sel + 2;
    C3DVector d; d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f;
    if (!s.UpdateDisplacePoints(d)) { std::printf("00761200_TEST FAIL\n"); return 1; }
    if (pts[0].X != 1.0f || pts[0].Y != 12.0f || pts[0].Z != 2.0f || pts[1].X != 1.0f || pts[1].Y != 10.0f || pts[2].X != 3.0f || pts[2].Z != 2.0f) { std::printf("00761200_TEST FAIL\n"); return 1; }
    s.SelectedPoints.m_pLast = sel;
    if (s.UpdateDisplacePoints(d) || pts[0].X != 1.0f) { std::printf("00761200_TEST FAIL\n"); return 1; }
    std::printf("00761200_TEST PASS\n");
    return 0;
}