#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C2DVector { float X, Y; C2DVector() { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } }; static C2DVector Sub(const C2DVector& a, const C2DVector& b) { C2DVector r; r.X = a.X - b.X; r.Y = a.Y - b.Y; return r; } struct C3DVector { float X, Y, Z; };
static C3DVector g_v; static long g_size = 0, g_flags = -1; struct COwner { virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { } virtual void SetRelativeTarget(const C3DVector* v, long s, long f) { g_v = *v; g_size = s; g_flags = f; } };
struct CTarget { COwner* m_pOwner; C2DVector m_origin; void SetTarget(const C3DVector* p) { C2DVector d = Sub(*(const C2DVector*)p, m_origin); C3DVector r; r.X = d.X; r.Y = d.Y; r.Z = p->Z; m_pOwner->SetRelativeTarget(&r, 12, 0); } };

int main() {
    COwner o; CTarget t; t.m_pOwner = &o; t.m_origin.X = 1; t.m_origin.Y = 2; C3DVector p; p.X = 5; p.Y = 7; p.Z = 9; t.SetTarget(&p); if (g_v.X != 4 || g_v.Y != 5 || g_v.Z != 9 || g_size != 12 || g_flags != 0) { std::printf("00a7d9c0_TEST FAIL\n"); return 1; }
    std::printf("00a7d9c0_TEST PASS\n"); return 0;
}