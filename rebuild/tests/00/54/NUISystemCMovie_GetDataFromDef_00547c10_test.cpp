#include <cstdio>
static int g_destroyed = 0;
class CWideString { public: const char* text; const CWideString& operator=(const CWideString& o) { text = o.text; return *this; } };
struct CMovieDef {
    long m_count; CWideString Movie; float Height; float Width; float Angle; bool PositionIsCenter;
    void Destroy() { ++g_destroyed; }
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
struct CDefPointer { CMovieDef* Data; ~CDefPointer() { if (Data) Data->ReleaseTemporary(); } const CMovieDef* operator->() const { return Data; } };
struct CMovie {
    CMovieDef* def;
    unsigned int Width, Height; bool PositionIsCenter; CWideString Movie; float Angle;
    CDefPointer LoadAndReturnDef() const { CDefPointer p; p.Data = def; ++def->m_count; return p; }
    void GetDataFromDef()
    {
        CDefPointer d = LoadAndReturnDef();
        Width = (unsigned int)d->Width; Height = (unsigned int)d->Height;
        PositionIsCenter = d->PositionIsCenter; Movie = d->Movie; Angle = d->Angle;
    }
};
int main()
{
    CMovieDef def; def.m_count = 0; def.Movie.text = "intro.xmv"; def.Height = 480.7f; def.Width = 640.2f; def.Angle = 0.5f; def.PositionIsCenter = true;
    CMovie m; m.def = &def; m.Width = m.Height = 0; m.PositionIsCenter = false; m.Movie.text = 0; m.Angle = 0;
    m.GetDataFromDef();
    if (m.Width != 640 || m.Height != 480 || !m.PositionIsCenter || m.Movie.text != def.Movie.text || m.Angle != 0.5f || g_destroyed != 1) { std::printf("00547c10_TEST FAIL\n"); return 1; }
    std::printf("00547c10_TEST PASS\n");
    return 0;
}