#include <cstdio>
#include "engine/CDisplayEngine.h"
class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawScreenFade {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);  // 0x006495A8
};
class CDrawLetterBoxBlackBars {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);  // 0x00644677
};
}
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
};
class CPlayerGui {
public:
    unsigned char                        _pad000[0x10];
    CMainGameComponent*                  Component;      // +0x10
    unsigned char                        _pad014[0x168 - 0x14];
    NPlayerGui::CDrawScreenFade*         PDrawScreenFade;          // +0x168 CCountedPointer data
    unsigned char                        _pad16c[0x208 - 0x16c];
    NPlayerGui::CDrawLetterBoxBlackBars* PDrawLetterBoxBlackBars;  // +0x208 CCountedPointer data
    void DrawScreenFade(const CEnginePrimitivePrepareInfo& info);
};
void CPlayerGui::DrawScreenFade(const CEnginePrimitivePrepareInfo& info)
{
    PDrawScreenFade->Draw(info);
    if (Component->PDisplayEngine->LetterBox[0])
        PDrawLetterBoxBlackBars->Draw(info);
}

static int g_fade, g_bars; static const CEnginePrimitivePrepareInfo* g_info;
void NPlayerGui::CDrawScreenFade::Draw(const CEnginePrimitivePrepareInfo& info) { ++g_fade; g_info = &info; if (g_bars) g_fade = 100; }
void NPlayerGui::CDrawLetterBoxBlackBars::Draw(const CEnginePrimitivePrepareInfo& info) { ++g_bars; if (&info != g_info) g_bars = 100; }
int main()
{
    static unsigned char gui_mem[0x300], eng_mem[sizeof(CDisplayEngine)];
    CPlayerGui* gui = (CPlayerGui*)gui_mem; CDisplayEngine* eng = (CDisplayEngine*)eng_mem;
    CMainGameComponent comp; comp.PDisplayEngine = eng;
    NPlayerGui::CDrawScreenFade fade; NPlayerGui::CDrawLetterBoxBlackBars bars;
    gui->Component = &comp; gui->PDrawScreenFade = &fade; gui->PDrawLetterBoxBlackBars = &bars;
    const CEnginePrimitivePrepareInfo* info = (const CEnginePrimitivePrepareInfo*)gui_mem;
    gui->DrawScreenFade(*info);
    bool ok = g_fade == 1 && g_bars == 0;
    eng->LetterBox[0] = 1;
    gui->DrawScreenFade(*info);
    ok = ok && g_fade == 2 && g_bars == 1;
    std::printf(ok ? "00437670_TEST PASS\n" : "FAIL\n");
    return 0;
}