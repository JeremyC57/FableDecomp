#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_GETEXTENTS)
#include "engine/C2DExtentsF.h"
#endif
#ifdef FABLE_RETAIL_C2DBOXF_SETTLPOS
#include "engine/C2DVector.h"
#endif
#ifdef FABLE_RETAIL_C2DBOXF_CLOCK_CORNERS_OWNER_PASS30
class C2DVector;
#endif
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
#ifdef FABLE_RETAIL_C2DBOXF_SETTLPOS
 // Retail opt-in (Codex pass22): both PDBs and donor020DF960..020DF983; original helper expands in0064BFBA.
 void SetTLPos(const C2DVector& pos) { TLX=pos.X; TLY=pos.Y; }
#endif
#ifdef FABLE_RETAIL_C2DBOXF
 // Retail opt-in (movie row 0x006286F0): FableWin.pdb ??0C2DBoxF@@QAE@MMMM@Z (debug only: inline in release; 0x006286F0
 // builds C2DBoxF(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT) on the stack, four stores, no call).
 C2DBoxF(float tlx,float tly,float brx,float bry):TLX(tlx),TLY(tly),BRX(brx),BRY(bry){}
#ifdef FABLE_RETAIL_C2DBOXF_INITIALISE
 // Retail opt-in (agentC43, row 0x005379F5 NUISystem::CList::ComputeChildrenVisibility): FableWin.pdb ?Initialise@C2DBoxF@@QAEXMMMM@Z and ?Initialise@C2DBoxF@@QAEXABV1@@Z
 // (inline in retail).
 void Initialise(float tlx,float tly,float brx,float bry){TLX=tlx;TLY=tly;BRX=brx;BRY=bry;}
 void Initialise(const C2DBoxF& box){TLX=box.TLX;TLY=box.TLY;BRX=box.BRX;BRY=box.BRY;}
#endif
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
#ifdef FABLE_RETAIL_C2DBOXF_GETTRPOS_PASS30
 // FableWin 020DDAA0 and retail 00643970: return the top-right corner by value.
 class C2DVector GetTRPos() const;
#endif
#if defined(FABLE_RETAIL_C2DBOXF_EXTENT_NAMED)
 // Overlay opt-in (wfA4, row 0x00BAA870): FableWin ?GetWidth@C2DBoxF@@QBEMXZ / ?GetHeight@C2DBoxF@@QBEMXZ are 13i/30b and 13i/31b at /Od,
 // two instructions more than the one-expression body (11i/22b): the difference stored in a named local and returned.
 float GetWidth() const { float width=BRX-TLX; return width; }
 float GetHeight() const { float height=BRY-TLY; return height; }
#else
 float GetWidth() const { return BRX-TLX; }
 float GetHeight() const { return BRY-TLY; }
#endif
#ifdef FABLE_RETAIL_C2DBOXF_GETEXTENTS
 // Original FableWin PDB exact GetExtents signature; donor0227D670 calls
 // GetHeight, GetWidth and the real C2DExtentsF(float,float) value constructor.
 C2DExtentsF GetExtents() const { return C2DExtentsF(GetWidth(),GetHeight()); }
#endif
#if defined(FABLE_RETAIL_C2DBOXF_EXTENTS) || defined(FABLE_RETAIL_C2DBOXF_EXTENTS_DECL)
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
#ifdef FABLE_RETAIL_C2DBOXF_MOVE
 // Nested opt-in (hand3c3, row 0x0041AFA0 NUISystem::CSprite::Draw): Ego_r.pdb / FableWin.pdb / Ego_d.pdb
 // ?Move@C2DBoxF@@QAEXABVC2DVector@@@Z (parameter `pos`; four statements in the Ego_d line table), an inline header function whose
 // out-of-line COMDAT copy is 0x0041A810 (landed; CMovie::Draw calls it) and which CSprite::Draw expands in place
 // (0x0041B141..0x0041B16F). Body follows the class.
 void Move(const class C2DVector& pos);
#endif
#ifdef FABLE_RETAIL_C2DBOXF_SETTERS
 // Nested opt-in (wfE9-9, row 0x00648820 NPlayerGui::CDrawMiniMap::Draw): FableWin.pdb ?SetTLPos@C2DBoxF@@QAEXABVC2DVector@@@Z,
 // ?SetWidth@C2DBoxF@@QAEXM@Z, ?SetHeight@C2DBoxF@@QAEXM@Z (FableWin 0x020DF960 / 0x020DF990 / 0x020DF9C0, emitted right after
 // CDrawMiniMap::Draw = first used there); inline in retail, whose NPlayerGui TU also keeps out-of-line COMDAT copies at 0x00643930
 // (copies X / Y into TLX / TLY), 0x00643950 (BRX = TLX + width), 0x00643960 (BRY = TLY + height). Bodies follow the class.
 // SetTLPos is the FABLE_RETAIL_C2DBOXF_SETTLPOS member when that opt-in is also set (same body).
#ifndef FABLE_RETAIL_C2DBOXF_SETTLPOS
 void SetTLPos(const class C2DVector& pos);
#endif
 void SetWidth(float width) { BRX = TLX + width; }
 void SetHeight(float height) { BRY = TLY + height; }
#endif
#ifdef FABLE_RETAIL_C2DBOXF_CORNERPOS
 // Nested opt-in (wfB6, row 0x005379F5 NUISystem::CList::ComputeChildrenVisibility): FableWin.pdb ?GetTLPos@C2DBoxF@@QBE?AVC2DVector@@XZ
 // and ?GetBRPos@C2DBoxF@@QBE?AVC2DVector@@XZ (inline in retail: the row copies the child box corners into two C2DVector
 // temporaries, 0x00537D9E..0x00537DAA and 0x00537DC1..0x00537DCA, and passes them to ContainsPoint). Bodies follow the class.
 class C2DVector GetTLPos() const;
 class C2DVector GetBRPos() const;
#ifdef FABLE_RETAIL_NAVQUADTREE_BUILD_PASS30
 class C2DVector GetTRPos() const;
 class C2DVector GetBLPos() const;
#endif
#endif
#endif
};
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_GETTRPOS_PASS30)
#include "engine/C2DVector.h"
#ifdef _MSC_VER
#pragma optimize("ty", on)
#endif
inline C2DVector C2DBoxF::GetTRPos() const { return C2DVector(BRX, TLY); }
#ifdef _MSC_VER
#pragma optimize("", on)
#endif
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CENTRE)
#include "engine/C2DVector.h"
inline C2DBoxF::C2DBoxF(const C2DVector& centre,float radius):TLX(centre.X-radius),TLY(centre.Y-radius),BRX(centre.X+radius),BRY(centre.Y+radius){}
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_EXTENTS)
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#if defined(FABLE_RETAIL_C2DBOXF_EXTENTS_OWNER_SPEED_PASS26) && defined(_MSC_VER)
// Exact native owner profile; genuine constructor body only.
#pragma optimize("t",on)
#pragma optimize("y",on)
#endif
#ifdef FABLE_RETAIL_C2DBOXF_EXTENTS_BODY_PASS26
// FableWin 0x018B13C0 writes the two point coordinates followed by the bounds sums.
inline C2DBoxF::C2DBoxF(const C2DVector& top_left, const C2DExtentsF& extents)
{
    TLX = top_left.X;
    TLY = top_left.Y;
    BRX = top_left.X + extents.X;
    BRY = top_left.Y + extents.Y;
}
#else
inline C2DBoxF::C2DBoxF(const C2DVector& top_left,const C2DExtentsF& extents):TLX(top_left.X),TLY(top_left.Y),BRX(top_left.X+extents.X),BRY(top_left.Y+extents.Y){}
#endif
#if defined(FABLE_RETAIL_C2DBOXF_EXTENTS_OWNER_SPEED_PASS26) && defined(_MSC_VER)
#pragma optimize("",on)
#endif
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CORNERS)
#include "engine/C2DVector.h"
#ifdef FABLE_RETAIL_C2DBOXF_CORNERS_FORCEINLINE
// Nested opt-in (agentSND, 2026-11-10, row 0x005A94BD CTCInventoryTrade::OpenInventory): the size TU of the trade screen expands the
// corners constructor in place (0x005A95E1..0x005A9600: the two C2DVector corners copied as dwords into the frame box, no call),
// which a plain `inline` does not give under optimize("s") (the call stays out of line); forced inline as engine/CDefPointer.h.
#include "fable_compiler.h"
FABLE_FORCEINLINE C2DBoxF::C2DBoxF(const C2DVector& tl,const C2DVector& br):TLX(tl.X),TLY(tl.Y),BRX(br.X),BRY(br.Y){}
#else
#if defined(FABLE_RETAIL_C2DBOXF_CLOCK_CORNERS_OWNER_PASS30) && defined(_MSC_VER)
// Inline-budget: the clock ctor calls the native 34-byte corners owner.
// Its visible input reads allow the caller to reuse the screen-vector slot.
#pragma auto_inline(off)
#pragma optimize("ty",on)
#endif
inline C2DBoxF::C2DBoxF(const C2DVector& tl,const C2DVector& br):TLX(tl.X),TLY(tl.Y),BRX(br.X),BRY(br.Y){}
#if defined(FABLE_RETAIL_C2DBOXF_CLOCK_CORNERS_OWNER_PASS30) && defined(_MSC_VER)
#pragma optimize("",on)
#pragma auto_inline(on)
#endif
#endif
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_MOVE)
#include "engine/C2DVector.h"
inline void C2DBoxF::Move(const C2DVector& pos)
{
    BRX += pos.X - TLX;
    BRY += pos.Y - TLY;
    TLX = pos.X;
    TLY = pos.Y;
}
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_SETTERS) && !defined(FABLE_RETAIL_C2DBOXF_SETTLPOS)
#include "engine/C2DVector.h"
inline void C2DBoxF::SetTLPos(const C2DVector& pos) { TLX = pos.X; TLY = pos.Y; }
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CORNERPOS)
#include "engine/C2DVector.h"
inline C2DVector C2DBoxF::GetTLPos() const { return C2DVector(TLX,TLY); }
inline C2DVector C2DBoxF::GetBRPos() const { return C2DVector(BRX,BRY); }
#ifdef FABLE_RETAIL_NAVQUADTREE_BUILD_PASS30
inline C2DVector C2DBoxF::GetTRPos() const { return C2DVector(BRX,TLY); }
inline C2DVector C2DBoxF::GetBLPos() const { return C2DVector(TLX,BRY); }
#endif
#endif
#if defined(FABLE_RETAIL_C2DBOXF) && defined(FABLE_RETAIL_C2DBOXF_CONTAINSPOINT_INLINE)
// Opt-in (wfB6, row 0x005379F5 NUISystem::CList::ComputeChildrenVisibility): ?ContainsPoint@C2DBoxF@@QBE_NABVC2DVector@@@Z is an
// inline header function in retail whose out-of-line COMDAT copy is 0x00535CD0 (the landed body): the row calls it (not
// expanded under optimize("s")) and reloads ecx = &window before the second call (0x00537DD1: a COMDAT callee may be replaced, so
// VC7.1 does not assume it preserves ecx), yet VC7.1 has read its body: it knows the callee neither writes memory nor keeps the box
// address, so the view-area box is built in place (ConvertX / ConvertY results stored straight into the box, 0x00537B7C..0x00537BE0).
// With the declaration alone the constructor arguments go through frame temporaries (+18 bytes).
// Body: the real text -- compiled out of line (default flags, probe address-of) it MATCHes 0x00535CD0 (wfB6 fix-up 2026-11-14).
// The PDB return type is bool (_N; the row's callers test al, 0x00537DB9 / 0x00537DD9) and `return a && b && c && d;` emits
// retail's `mov eax,1` / `xor eax,eax`; the landed 0x00535CD0 row returns long with if/return 1/0 (same bytes, contradicts the PDB),
// and that if/return spelling with bool literals gives `mov al,1` (63v66), so it is not used here.
#include "engine/C2DVector.h"
inline bool C2DBoxF::ContainsPoint(const C2DVector& point) const
{
    return point.X >= TLX && point.X < BRX && point.Y >= TLY && point.Y < BRY;
}
#endif
#endif
