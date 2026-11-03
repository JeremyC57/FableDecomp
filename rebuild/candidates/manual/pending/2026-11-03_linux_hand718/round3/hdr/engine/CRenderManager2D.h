#ifndef FABLE_ENGINE_CRENDERMANAGER2D_H
#define FABLE_ENGINE_CRENDERMANAGER2D_H
// Retail CRenderManager2D (movie row 0x006286F0). RTTI (vtable 0x0129B9EC) CRenderManager2D : CRenderManagerCore, offset 0.
// Size: the only constructor 0x009D9A30 calls ??0CRenderManagerCore (0x00A0AFE0), stores vtable 0x0129B9EC and constructs
// its own members from +0x3D20 through +0x3E9C; CRenderManager (engine/CRenderManager.h) adds no member, and its creator
// 0x009C0FD0 allocates 0x3EA4 bytes. The own members (Ego_r.pdb 0x21C bytes with Dinkumware containers) are opaque.
// Member functions: Ego_r.pdb + FableWin.pdb spellings; retail addresses follow the Ego_r donor order
// (DrawBufferedPolys 0x00B2D610 < Render2DDrawList 0x00B2E390 < Draw2DTexture(UV) 0x00B30450).
#ifndef FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#endif
#include "engine/CRenderManagerCore.h"
class C2DBoxF; class CUVBox; class CRGBColour; class CTexture;
#ifdef FABLE_RETAIL_CRENDERMANAGER2D_TEXT
#include "fable_string.h"
class C2DVector; class CFontBank;
#endif
class CRenderManager2D : public CRenderManagerCore {
private:
    unsigned char _pad_0x3d20[0x3ea4 - 0x3d20];                          // +0x3D20 own members (opaque)
public:
    void DrawBufferedPolys();                                            // 0x009D9C80 ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ
    void Render2DDrawList(bool);                                         // 0x009DA9F0 ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z
    bool Draw2DTexture(const C2DBoxF&,const CUVBox&,const CRGBColour&,CTexture&,unsigned long);
                                                                         // 0x009DC870 ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCUVBox@@ABVCRGBColour@@AAVCTexture@@K@Z
#ifdef FABLE_RETAIL_CRENDERMANAGER2D_TEXT
    // Retail opt-in (chunk 718, row 0x00487570 CPlayer::DrawFreeCamDebug): FableWin.pdb ?Draw2DText@CRenderManager2D@@QAE_NABVC2DVector@@VCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z
    // (the landed body 0x009DD8F0, `ret 14h`; the string travels by value, built in its argument slot). A declaration only.
    bool Draw2DText(const C2DVector&,CWideString,const CFontBank&,const CRGBColour&,unsigned long);   // 0x009DD8F0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager2D) == 0x3ea4);
#endif // FABLE_ENGINE_CRENDERMANAGER2D_H
