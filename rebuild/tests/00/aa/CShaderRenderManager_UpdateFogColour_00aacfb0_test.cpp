// Behaviour test for 00aacfb0: src/00aacfb0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CShaderRenderManager::UpdateFogColour()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CShaderRenderManager::UpdateFogColour() -- when the owner's device exists, copy three colour vectors (+4, +0x1c, +0x10) into locals and hand them to the device's fog target (slot 12)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACFB0, length 108 bytes. Callees: device virtual slot 6 (+0x18) returns the fog target, its slot 12 (+0x30).
// Shape: memberwise copies need a user copy ctor on the vector; the owner pointer is a named local (kept in eax).
class C3DVector { public: float x, y, z; C3DVector(const C3DVector& v) : x(v.x), y(v.y), z(v.z) {} };
struct CFogTarget {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11();
    virtual void SetFog(const C3DVector& a, const C3DVector& b, const C3DVector& c);   // slot 12
};
struct CDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual CFogTarget* GetFogTarget();   // slot 6
};
struct COwner { unsigned char _pad[0x40]; CDevice* m_pDevice; };
class CShaderRenderManager { public:
    void*     _vf;
    C3DVector m_a;      // +0x04
    C3DVector m_c;      // +0x10
    C3DVector m_b;      // +0x1c
    COwner*   m_pOwner; // +0x28
    void UpdateFogColour();
};
void CShaderRenderManager::UpdateFogColour()
{
    COwner* const& owner = m_pOwner;
    if (owner->m_pDevice) {
        C3DVector a = m_a;
        C3DVector b = m_b;
        C3DVector c = m_c;
        owner->m_pDevice->GetFogTarget()->SetFog(a, b, c);
    }
}

// ---- stubs and driver ----
void CFogTarget::v00() {}
void CFogTarget::v01() {}
void CFogTarget::v02() {}
void CFogTarget::v03() {}
void CFogTarget::v04() {}
void CFogTarget::v05() {}
void CFogTarget::v06() {}
void CFogTarget::v07() {}
void CFogTarget::v08() {}
void CFogTarget::v09() {}
void CFogTarget::v10() {}
void CFogTarget::v11() {}
void CDevice::v00() {}
void CDevice::v01() {}
void CDevice::v02() {}
void CDevice::v03() {}
void CDevice::v04() {}
void CDevice::v05() {}
static int g_calls = 0; static float g_a[3], g_b[3], g_c[3];
struct CTarget : CFogTarget { void SetFog(const C3DVector& a, const C3DVector& b, const C3DVector& c)
{ ++g_calls; g_a[0] = a.x; g_a[1] = a.y; g_a[2] = a.z; g_b[0] = b.x; g_b[1] = b.y; g_b[2] = b.z; g_c[0] = c.x; g_c[1] = c.y; g_c[2] = c.z; } };
void CFogTarget::SetFog(const C3DVector&, const C3DVector&, const C3DVector&) {}
static CTarget g_target;
struct CDev : CDevice { CFogTarget* GetFogTarget() { return &g_target; } };
CFogTarget* CDevice::GetFogTarget() { return 0; }
int main()
{
    static CDev dev; static COwner owner; memset(&owner, 0, sizeof owner);
    static unsigned char ms[sizeof(CShaderRenderManager)]; CShaderRenderManager* m = (CShaderRenderManager*)ms;
    m->m_a.x = 1; m->m_a.y = 2; m->m_a.z = 3; m->m_b.x = 4; m->m_b.y = 5; m->m_b.z = 6; m->m_c.x = 7; m->m_c.y = 8; m->m_c.z = 9;
    m->m_pOwner = &owner;
    m->UpdateFogColour(); CHECK(g_calls == 0);           // no device
    owner.m_pDevice = &dev;
    m->UpdateFogColour(); CHECK(g_calls == 1);
    CHECK(g_a[0] == 1 && g_a[2] == 3); CHECK(g_b[0] == 4 && g_b[1] == 5 && g_b[2] == 6); CHECK(g_c[0] == 7 && g_c[2] == 9);
    if (fails == 0) printf("00aacfb0_TEST PASS\n"); else printf("00aacfb0_TEST FAIL\n");
    return fails;
}