#include <cstdio>
// Model of the implicit aggregate destructor @ 0x00708A30: counted release, interpolator, two CBaseClass members (reverse order).
static int g_order[8], g_n, g_deleted;
struct CCountedObject { long m_refCount; void Delete() { ++g_deleted; } };
struct CCountedPointer { CCountedObject* m_pObject; ~CCountedPointer() { CCountedObject* o = m_pObject; if (o) { if (--o->m_refCount == 0) o->Delete(); m_pObject = 0; } g_order[g_n++] = 1; } };
class C3DAnimationInterpolator { public: ~C3DAnimationInterpolator() { g_order[g_n++] = 2; } };
class CBaseClass { public: int tag; ~CBaseClass() { g_order[g_n++] = tag; } };
struct X { CBaseClass m_first; CBaseClass m_second; C3DAnimationInterpolator m_interpolator; CCountedPointer m_pAnimation; ~X(); };
X::~X() {}
int main()
{
    CCountedObject obj = { 1 };
    { X x; x.m_first.tag = 4; x.m_second.tag = 3; x.m_pAnimation.m_pObject = &obj; }
    if (g_n != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4 || g_deleted != 1) { std::printf("00708a30_TEST FAIL\n"); return 1; }
    g_n = 0; { X x; x.m_first.tag = 4; x.m_second.tag = 3; x.m_pAnimation.m_pObject = 0; }
    if (g_deleted != 1 || g_n != 4) { std::printf("00708a30_TEST FAIL\n"); return 1; }
    std::printf("00708a30_TEST PASS\n"); return 0;
}