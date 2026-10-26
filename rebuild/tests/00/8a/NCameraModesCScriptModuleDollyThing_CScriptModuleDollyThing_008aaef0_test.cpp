#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
class C3DVector { public: float X, Y, Z; }; struct SHandle_008aaef0 { long m_id; };
struct CScriptModuleBase { virtual void v00() { } unsigned char _pad[0x48]; CScriptModuleBase() { ++g_base; } };
namespace NCameraModes { struct CScriptModuleDollyThing : CScriptModuleBase { void* m_pThing; float m_offsetX, m_offsetY, m_offsetZ; SHandle_008aaef0 m_target;
    CScriptModuleDollyThing(void* t, const C3DVector& o, const SHandle_008aaef0& h) : CScriptModuleBase(), m_pThing(t), m_offsetX(o.X), m_offsetY(o.Y), m_offsetZ(o.Z), m_target(h) { } }; }
int main() {
    int thing = 0; C3DVector v = { 1, 2, 3 }; SHandle_008aaef0 h = { 9 }; NCameraModes::CScriptModuleDollyThing d(&thing, v, h);
    if (g_base != 1 || d.m_pThing != &thing || d.m_offsetX != 1 || d.m_offsetZ != 3 || d.m_target.m_id != 9) { std::printf("008aaef0_TEST FAIL\n"); return 1; }
    std::printf("008aaef0_TEST PASS\n"); return 0;
}