// CPlayerGui::DrawScreenFade(const CEnginePrimitivePrepareInfo& info) -- draw the screen fade, then the letterbox bars while letterbox mode is on
// Leaf: ?DrawScreenFade@CPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe @ 0x00437670, 50 bytes, `ret 4`; Ego_r.pdb ?DrawScreenFade@CPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees NPlayerGui::CDrawScreenFade::Draw (0x006495A8, on PDrawScreenFade +0x168) and
// NPlayerGui::CDrawLetterBoxBlackBars::Draw (0x00644677, on PDrawLetterBoxBlackBars +0x208), the second only when
// Component (+0x10) -> PDisplayEngine (CMainGameComponent +0x28) -> LetterBox (CDisplayEngine +0x70) first byte is set.
// Layout: retail CPlayerGui (engine/CPlayerGui.h carries the Ego_r.pdb offsets, 8 higher past SubtitleRenderer:
// PDrawScreenFade +0x170 / PDrawLetterBoxBlackBars +0x210 there); CDisplayEngine from engine/CDisplayEngine.h.
// Shape: /O2.
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