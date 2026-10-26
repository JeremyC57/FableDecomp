#include <cstdio>
static int g_destroyed = 0;
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
struct CUIDef {
    long m_count; C2DBoxF TextWindow; float ScrollingSpeed; float InputDelay;
    virtual void Destroy() { ++g_destroyed; }
    virtual ~CUIDef() { }
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
struct CDefPointer_CUIDef {
    const CUIDef* Data;
    CDefPointer_CUIDef(const CUIDef* d) : Data(d) { if (Data) ++const_cast<CUIDef*>(Data)->m_count; }
    CDefPointer_CUIDef(const CDefPointer_CUIDef& o) : Data(o.Data) { if (Data) ++const_cast<CUIDef*>(Data)->m_count; }
    ~CDefPointer_CUIDef() { if (Data) const_cast<CUIDef*>(Data)->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
};
static CUIDef g_def;
struct CComponent {
    CDefPointer_CUIDef LoadAndReturnDef() const { return CDefPointer_CUIDef(&g_def); }
};
struct CZoomingComponent : CComponent {
    bool Panning; bool Zooming; float InputDelay; float ScrollingSpeed;
    void GetDataFromDef()
    {
        CDefPointer_CUIDef def = LoadAndReturnDef();
        InputDelay = def->InputDelay; ScrollingSpeed = def->ScrollingSpeed; Panning = false; Zooming = false;
    }
};
int main()
{
    g_def.m_count = 2; g_def.InputDelay = 0.25f; g_def.ScrollingSpeed = 3.5f;
    CZoomingComponent z; z.Panning = true; z.Zooming = true; z.InputDelay = 0; z.ScrollingSpeed = 0;
    z.GetDataFromDef();
    bool ok = z.InputDelay == 0.25f && z.ScrollingSpeed == 3.5f && !z.Panning && !z.Zooming && g_def.m_count == 2 && g_destroyed == 0;
    g_def.m_count = 0; z.GetDataFromDef();
    ok = ok && g_destroyed == 1;
    if (!ok) { std::printf("00546aa0_TEST FAIL\n"); return 1; }
    std::printf("00546aa0_TEST PASS\n");
    return 0;
}