#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
#ifdef FABLE_RETAIL_C2DBOXF
 // Retail opt-in (movie row 0x006286F0): FableWin.pdb ??0C2DBoxF@@QAE@MMMM@Z (debug only: inline in release; 0x006286F0
 // builds C2DBoxF(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT) on the stack, four stores, no call).
 C2DBoxF(float tlx,float tly,float brx,float bry):TLX(tlx),TLY(tly),BRX(brx),BRY(bry){}
#ifdef FABLE_RETAIL_C2DBOXF_COPY
 // Nested opt-in (chunk 853, row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): the authored copy constructor, FableWin.pdb
 // ??0C2DBoxF@@QAE@ABV0@@Z (0x01A664E0; inline in release). Member-wise float copies let VC7.1 build the CThingFilter_InArea copy of
 // area_in_floating_point straight from the four fild results (retail 0x0050AC81..0x0050ACB1: fstp into the filter, no dword moves).
 C2DBoxF(const C2DBoxF& other):TLX(other.TLX),TLY(other.TLY),BRX(other.BRX),BRY(other.BRY){}
#endif
#ifdef FABLE_RETAIL_C2DBOXF_CLIPTO
 // Retail opt-in (chunk 802, row 0x0066CF00 CRegionMinimap::PopulateRegionMinimap): Ego_r.pdb / FableWin.pdb ?ClipTo@C2DBoxF@@QAEXABV1@@Z
 // (Ego_r 0x006A11F0), out of line in retail at 0x0066C560 (the caller passes the region box by address, `this` the building box).
 void ClipTo(const C2DBoxF& other);                                     // 0x0066C560
#endif
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
#ifdef FABLE_RETAIL_C2DBOXF_EXTENTS
 // Nested opt-in (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay): FableWin.pdb ??0C2DBoxF@@QAE@ABVC2DVector@@ABVC2DExtentsF@@@Z
 // (0x018B13C0, the top-left corner and the extents; inline in retail: the loading-screen bar pieces store TLX / TLY and the two
 // sums, 0x00498A68 .. 0x00498A8B). Needs C2DVector / C2DExtentsF (the body follows the class).
 C2DBoxF(const class C2DVector& top_left,const class C2DExtentsF& extents);
#endif
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
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_EXTENTS)
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
inline C2DBoxF::C2DBoxF(const C2DVector& top_left,const C2DExtentsF& extents):TLX(top_left.X),TLY(top_left.Y),BRX(top_left.X+extents.X),BRY(top_left.Y+extents.Y){}
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CORNERS)
#include "engine/C2DVector.h"
inline C2DBoxF::C2DBoxF(const C2DVector& tl,const C2DVector& br):TLX(tl.X),TLY(tl.Y),BRX(br.X),BRY(br.Y){}
#endif
#endif
