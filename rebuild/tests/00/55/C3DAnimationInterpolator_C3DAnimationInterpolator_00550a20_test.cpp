#include <cstdio>
static int g_base = 0;
namespace N_0099a2d0 { struct CBase { virtual void v00() { } CBase() { ++g_base; } }; } using N_0099a2d0::CBase;
struct SCountedAnimation_00550a20 { long m_a; long m_refCount; };
class C3DAnimationBlendState { public: virtual void v00() { } SCountedAnimation_00550a20* m_pAnimation; C3DAnimationBlendState(SCountedAnimation_00550a20* p) : m_pAnimation(p) { }
    C3DAnimationBlendState(const C3DAnimationBlendState& o) : m_pAnimation(o.m_pAnimation) { if (m_pAnimation) ++m_pAnimation->m_refCount; } };
class C3DAnimationInterpolator : public CBase { public: C3DAnimationBlendState m_from; C3DAnimationBlendState m_to;
    C3DAnimationInterpolator(const C3DAnimationBlendState& f, const C3DAnimationBlendState& t) : CBase(), m_from(f), m_to(t) { } };
int main() {
    SCountedAnimation_00550a20 a1 = { 0, 1 }, a2 = { 0, 5 }; C3DAnimationBlendState f(&a1), t(&a2), n(0);
    C3DAnimationInterpolator i(f, t); if (i.m_from.m_pAnimation != &a1 || i.m_to.m_pAnimation != &a2 || a1.m_refCount != 2 || a2.m_refCount != 6 || g_base != 1) { std::printf("00550a20_TEST FAIL\n"); return 1; }
    C3DAnimationInterpolator j(n, f); if (j.m_from.m_pAnimation != 0 || a1.m_refCount != 3) { std::printf("00550a20_TEST FAIL\n"); return 1; }
    std::printf("00550a20_TEST PASS\n"); return 0;
}