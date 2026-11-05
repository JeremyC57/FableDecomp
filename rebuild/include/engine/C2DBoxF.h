#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
#ifdef FABLE_RETAIL_C2DBOXF
 // Retail opt-in (movie row 0x006286F0): FableWin.pdb ??0C2DBoxF@@QAE@MMMM@Z (debug only: inline in release; 0x006286F0
 // builds C2DBoxF(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT) on the stack, four stores, no call).
 C2DBoxF(float tlx,float tly,float brx,float bry):TLX(tlx),TLY(tly),BRX(brx),BRY(bry){}
#ifdef FABLE_RETAIL_C2DBOXF_CORNERS
 // Chunk 749 (row 0x005502E0 NUISystem::CMesh::Draw): FableWin.pdb ??0C2DBoxF@@QAE@ABVC2DVector@@0@Z (top-left, bottom-right;
 // inline in retail).
 C2DBoxF(const class C2DVector& tl,const class C2DVector& br);
#endif
 // chunk 726: ?ContainsPoint@C2DBoxF@@QBE_NABVC2DVector@@@Z, out of line at 0x00535CD0 (landed); called by
 // NUISystem::CList::ComputeChildrenVisibility 0x005379F5.
 // (the elaborated "class C2DVector" declares ::C2DVector, so the default path gains no forward declaration).
 bool ContainsPoint(const class C2DVector& point) const;
#ifndef FABLE_RETAIL_C2DBOXF_CORNERS
 // chunk 751 (row 0x00493AC2 CEnginePrimitive2DSprite ctor): Ego_r.pdb / FableWin.pdb ??0C2DBoxF@@QAE@ABVC2DVector@@0@Z, out of
 // line at 0x004371A0 (copies the two corners; the landed clock-sprite row 0x0065595C binds it as a pointer pair, which no PDB has).
 // Chunk 779 (row 0x00542A30): guarded so that FABLE_RETAIL_C2DBOXF_CORNERS (the inline copy above, chunk 749) is not a redeclaration.
 C2DBoxF(const class C2DVector& top_left,const class C2DVector& bottom_right);
#endif
 // chunk 726: the PDB accessors (FableWin ?GetTLX@C2DBoxF@@QBEMXZ .. ?GetHeight@C2DBoxF@@QBEMXZ), inline in retail:
 // NUISystem::CScrollingViewport::Update 0x0053F1C0 copies Window.GetTLX() into a frame word before the GetUIScale call
 // and computes GetWidth() / GetHeight() (fld BRX ; fsub TLX) ahead of it.
 float GetTLX() const { return TLX; }
 float GetTLY() const { return TLY; }
 float GetBRX() const { return BRX; }
 float GetBRY() const { return BRY; }
 float GetWidth() const { return BRX-TLX; }
 float GetHeight() const { return BRY-TLY; }
#ifdef FABLE_RETAIL_C2DBOXF_CENTRE
 // Nested opt-in (chunk 768, row 0x00969E70 CAIStateGroup_SleepOnFloor::IsRunnable): FableWin.pdb ??0C2DBoxF@@QAE@ABVC2DVector@@M@Z
 // (the square of half-size `radius` round `centre`; inline in retail: four x87 stores, no call) and Ego_r.pdb
 // ?ToC2DBoxI@C2DBoxF@@QBE?AVC2DBoxI@@XZ (out of line at 0x00A55A30, landed; C2DBoxI by hidden pointer). Needs C2DVector / C2DBoxI.
 C2DBoxF(const class C2DVector& centre,float radius);
 class C2DBoxI ToC2DBoxI() const;                                       // 0x00A55A30
#endif
#endif
};
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CENTRE)
#include "engine/C2DVector.h"
inline C2DBoxF::C2DBoxF(const C2DVector& centre,float radius):TLX(centre.X-radius),TLY(centre.Y-radius),BRX(centre.X+radius),BRY(centre.Y+radius){}
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CORNERS)
#include "engine/C2DVector.h"
inline C2DBoxF::C2DBoxF(const C2DVector& tl,const C2DVector& br):TLX(tl.X),TLY(tl.Y),BRX(br.X),BRY(br.Y){}
#endif
#endif
