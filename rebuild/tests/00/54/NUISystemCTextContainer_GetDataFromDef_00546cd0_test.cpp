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
struct CTextContainer : CComponent {
    C2DBoxF Window;
    void GetDataFromDef() { CDefPointer_CUIDef def = LoadAndReturnDef(); Window = def->TextWindow; }
};
int main()
{
    g_def.m_count = 0; g_def.TextWindow.TLX = 1; g_def.TextWindow.TLY = 2; g_def.TextWindow.BRX = 3; g_def.TextWindow.BRY = 4;
    CTextContainer c; c.Window.TLX = c.Window.TLY = c.Window.BRX = c.Window.BRY = 0;
    c.GetDataFromDef();
    bool ok = c.Window.TLX == 1 && c.Window.TLY == 2 && c.Window.BRX == 3 && c.Window.BRY == 4 && g_destroyed == 1 && g_def.m_count == 0;
    g_def.m_count = 1; c.GetDataFromDef();
    ok = ok && g_destroyed == 1 && g_def.m_count == 1;
    if (!ok) { std::printf("00546cd0_TEST FAIL\n"); return 1; }
    std::printf("00546cd0_TEST PASS\n");
    return 0;
}