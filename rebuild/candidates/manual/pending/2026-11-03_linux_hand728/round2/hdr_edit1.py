import sys
H="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/chunk728b/hdr/"
def ed(f, pairs):
    p=H+f; s=open(p).read()
    for a,b in pairs:
        assert s.count(a)==1, (f, a[:60], s.count(a))
        s=s.replace(a,b,1)
    open(p,"w").write(s)
# --- fable_game.h: CMainGameComponent retail branch
ed("fable_game.h",[
("""    unsigned char _pad_0x70[0x16178]; // All original members LastInterpolationInfo through DebugNoFramesUnableToRender.""",
"""    unsigned char _pad_0x70[0x160D4]; // Original members LastInterpolationInfo through LocalSeed.
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): the two Ego_r.pdb font pointers typed (PDebugFont +0x16144,
    // PCutSceneMainFont +0x1614C, both CCountedPointer<CFontBank>); GlobalUpdate reads PCutSceneMainFont.Data (+0x1614C) in place
    // for CSubtitleRenderer::SetText. Same offsets as engine/CMainGameComponent.h; the layout and size are unchanged.
    CCountedPointer<CFontBank> PDebugFont;                               // +0x16144
    CCountedPointer<CFontBank> PCutSceneMainFont;                        // +0x1614C
    unsigned char _pad_0x16154[0x94]; // Original members EventPackageFile through DebugNoFramesUnableToRender."""),
("""class CASoundBank; class CDisplayEngine; class CGamePlayerInterface; class CLUA; class CPlayerManager; class CWorld;""",
"""class CASoundBank; class CDisplayEngine; class CGamePlayerInterface; class CLUA; class CPlayerManager; class CWorld; class CFontBank;"""),
("""    const CWorld& PeekWorld() const { return *PWorld; }
};""",
"""    const CWorld& PeekWorld() const { return *PWorld; }
    // Chunk 728 round 2 (row 0x006D0830): FableWin.pdb ?GetWorld@CMainGameComponent@@QAEAAVCWorld@@XZ (0x018A3A80),
    // ?GetDisplayEngine@CMainGameComponent@@QAEAAVCDisplayEngine@@XZ (0x01882EC0) and ?PeekCutSceneMainFont@CMainGameComponent@@QBEABV
    // CFontBank@@XZ (0x0187F030), inline in Ego_r.pdb and retail (GlobalUpdate reads +0x24 / +0x28 / +0x1614C in place); and the
    // out-of-line ?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ 0x00416225 (`mov eax,[ecx+48h] ; ret` = CurrentServerFrame;
    // Ego_r order GetCurrentServerFrame 0x0041548A / GetCurrentFrameStartGameTime 0x0041548E / GetGameStartTime 0x00415492 =
    // retail 0x00416225 / 0x00416229 (+0x58) / 0x0041622D (+0x60)).
    CWorld& GetWorld() { return *PWorld; }
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
    const CFontBank& PeekCutSceneMainFont() const { return *PCutSceneMainFont; }
    long GetCurrentServerFrame() const;                                  // 0x00416225
};"""),
])
# --- CDisplayEngine.h
ed("engine/CDisplayEngine.h",[
("""    void PostChangeResolution(unsigned long, unsigned long, unsigned long);   // 0x00434A20 ?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z
#endif""",
"""    void PostChangeResolution(unsigned long, unsigned long, unsigned long);   // 0x00434A20 ?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb ?EndScreenFadeOut@CDisplayEngine@@QAEXXZ (out of line,
    // 0x00434C90, the landed row), ?IsScreenFadeOut@CDisplayEngine@@QBE_NXZ and ?IsScreenFullyFadedOut@CDisplayEngine@@QBE_NXZ (Ego_r /
    // FableWin). IsScreenFullyFadedOut's retail COMDAT copy 0x004A9210 tests Active +0xBC, Opening +0xC9, Closing +0xC8 in that
    // order, the order GlobalUpdate expands it in place; IsScreenFadeOut is the Active test alone (GlobalUpdate 0x006D0FD9).
    void EndScreenFadeOut();                                            // 0x00434C90
    bool IsScreenFadeOut() const { return ScreenFadeOutInfo.Active; }
    bool IsScreenFullyFadedOut() const { return ScreenFadeOutInfo.Active && !ScreenFadeOutInfo.Opening && !ScreenFadeOutInfo.Closing; }
#endif"""),
("""    unsigned char           ScreenFadeOutInfo[0x1c];         // +0xbc CFadeInFadeOutBase""",
"""#ifdef FABLE_RETAIL_CDISPLAYENGINE
    CFadeInFadeOutBase      ScreenFadeOutInfo;               // +0xbc (chunk 728 round 2: typed in the retail branch)
#else
    unsigned char           ScreenFadeOutInfo[0x1c];         // +0xbc CFadeInFadeOutBase
#endif"""),
("""class CWorld;

#pragma pack(push, 1)
class CDisplayEngine {""","""class CWorld;
#ifdef FABLE_RETAIL_CDISPLAYENGINE
#include "engine/CFadeInFadeOutBase.h"
#endif

#pragma pack(push, 1)
class CDisplayEngine {"""),
])
