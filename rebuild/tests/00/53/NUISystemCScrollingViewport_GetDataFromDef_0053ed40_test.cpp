// Behaviour test for NUISystem::CScrollingViewport::GetDataFromDef (0x0053ED40): copies speed + window from the def and drops the temporary's reference (destroying at zero).
#include <cstdio>
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
static int g_destroyed = 0;
struct CUIDef {
    long m_count;
    C2DBoxF TextWindow;
    float ScrollingSpeed;
    virtual ~CUIDef() {}
    virtual void Destroy() { ++g_destroyed; }
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
struct CDefPointer_CUIDef {
    CUIDef* Data;
    explicit CDefPointer_CUIDef(CUIDef* d) : Data(d) { if (Data) ++Data->m_count; }
    CDefPointer_CUIDef(const CDefPointer_CUIDef& o) : Data(o.Data) { if (Data) ++Data->m_count; }
    ~CDefPointer_CUIDef() { if (Data) Data->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
};
struct CScrollingViewport {
    CUIDef* m_def;
    float ScrollingSpeed;
    C2DBoxF Window;
    virtual ~CScrollingViewport() {}
    virtual CDefPointer_CUIDef LoadAndReturnDef() const { return CDefPointer_CUIDef(m_def); }
    void GetDataFromDef()
    {
        CDefPointer_CUIDef def = LoadAndReturnDef();
        ScrollingSpeed = def->ScrollingSpeed;
        Window = def->TextWindow;
    }
};
int main()
{
    CUIDef def;
    def.m_count = 1;
    def.ScrollingSpeed = 3.5f;
    def.TextWindow.TLX = 1.0f; def.TextWindow.TLY = 2.0f; def.TextWindow.BRX = 30.0f; def.TextWindow.BRY = 40.0f;
    CScrollingViewport v;
    v.m_def = &def;
    v.ScrollingSpeed = 0.0f;
    v.GetDataFromDef();
    if (v.ScrollingSpeed != 3.5f || v.Window.BRX != 30.0f || v.Window.TLY != 2.0f || def.m_count != 1 || g_destroyed != 0) { std::printf("0053ed40_TEST FAIL\n"); return 1; }
    def.m_count = 0;   // the def's last owner is the temporary: it is destroyed when released
    v.GetDataFromDef();
    if (g_destroyed != 1) { std::printf("0053ed40_TEST FAIL\n"); return 1; }
    std::printf("0053ed40_TEST PASS\n");
    return 0;
}