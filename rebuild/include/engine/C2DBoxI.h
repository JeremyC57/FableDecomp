#ifndef FABLE_ENGINE_C2DBOXI_H
#define FABLE_ENGINE_C2DBOXI_H
// Original public fields and user default constructor from Ego_r TPI.
#include "engine/C2DCoordI.h"
#if defined(FABLE_RETAIL_C2DBOXI_OVERLAPPING_BODY) && defined(_MSC_VER)
#include <stdlib.h>                                          // chunk 1013: abs() in the Overlapping body
#endif
#ifdef FABLE_RETAIL_C2DBOXI
#include "engine/C2DExtentsI.h"
#endif
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
// Original Ego_r TPI0xb186; centre/radius arithmetic is native0074a685 onward.
#ifdef FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
// Retail opt-in (Codex2026-11-10): FableWin01addbb0 has direct four field initialization.
C2DBoxI(const C2DCoordI& p,long radius):TLX(p.X-radius),TLY(p.Y-radius),BRX(p.X+radius),BRY(p.Y+radius){}
#else
C2DBoxI(const C2DCoordI& p,long radius){long x=p.X;long y=p.Y;Initialise(x-radius,y-radius,x+radius,y+radius);}
#endif
void Initialise(long tlx,long tly,long brx,long bry){TLX=tlx;TLY=tly;BRX=brx;BRY=bry;}
#ifdef FABLE_RETAIL_C2DBOXI_CTORS
// Retail opt-in (chunk 792, row 0x00AB8E10 CStaticFontBank::CStaticFontBank): Ego_r.pdb ??0C2DBoxI@@QAE@JJJJ@Z and
// ??0C2DBoxI@@QAE@ABVC2DCoordI@@ABVC2DExtentsI@@@Z, inline (no out-of-line copy bound in Fable.exe).
C2DBoxI(long tlx,long tly,long brx,long bry){Initialise(tlx,tly,brx,bry);}
C2DBoxI(const C2DCoordI& tl,const C2DExtentsI& extents){Initialise(tl.X,tl.Y,tl.X+extents.X,tl.Y+extents.Y);}
#endif
#ifdef FABLE_RETAIL_C2DBOXI
// Header wave 2026-11-03 (row 0x00821010 reads the brush corners): Ego_r.pdb ?GetTLPos@C2DBoxI@@QBE?AVC2DCoordI@@XZ and
// ?GetBRPos@C2DBoxI@@QBE?AVC2DCoordI@@XZ, inline (no out-of-line copy in Fable.exe).
C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}
C2DCoordI GetBRPos()const{return C2DCoordI(BRX,BRY);}
// Chunk 756 (row 0x0066C7B0 CRegionMinimap::InitialiseRegionBox): Ego_r.pdb ?GetExtents@C2DBoxI@@QBE?AVC2DExtentsI@@XZ, inline.
C2DExtentsI GetExtents()const{return C2DExtentsI(BRX-TLX,BRY-TLY);}
// Chunk 756 (row 0x0049C700): Ego_r.pdb ?ClipTo@C2DBoxI@@QAEXABV1@@Z, out of line in retail (0x0049C810).
void ClipTo(const C2DBoxI& bounds);
// Chunk 756 (row 0x0049C700, parked): the memberwise user copy ctor of the landed CWorldMap twin 0x004FC4D0 (unconfirmed in the TPI).
#ifdef FABLE_RETAIL_C2DBOXI_COPY
C2DBoxI(const C2DBoxI& o):TLX(o.TLX),TLY(o.TLY),BRX(o.BRX),BRY(o.BRY){}
#endif
#endif
#if defined(FABLE_RETAIL_C2DBOXI_OVERLAPPING_BODY) && defined(_MSC_VER)
// Nested opt-in (chunk 1013, row 0x004FF150 CWorldMap::GetAllMapsInArea): Ego_r.pdb / FableWin.pdb ?Overlapping@C2DBoxI@@QBE_NABV1@@Z,
// out of line in retail at 0x004FAB40 (landed), which sits in the CWorldMap.cpp object: its body is visible to the CWorldMap rows, so
// VC7.1 knows the box it is given is only read (0x004FF150 keeps the clamped box.TLX / TLY in ebx / edi across AreaCoversOneLevel;
// with a declaration only they are reloaded). The landed 0x004FAB40 separating-axis test, kept out of line (noinline).
__declspec(noinline) bool Overlapping(const C2DBoxI& other) const   // 0x004FAB40
{
    if (abs((TLX + (BRX - TLX) / 2) - (other.TLX + (other.BRX - other.TLX) / 2)) <= (BRX - TLX) / 2 + (other.BRX - other.TLX) / 2)
    {
        long dy = abs((TLY + (BRY - TLY) / 2) - (other.TLY + (other.BRY - other.TLY) / 2));
        if (dy <= (BRY - TLY) / 2 + (other.BRY - other.TLY) / 2)
            return true;
    }
    return false;
}
#elif defined(FABLE_RETAIL_C2DBOXI_OVERLAPPING) || defined(FABLE_RETAIL_C2DBOXI_OVERLAPPING_BODY)
// Nested opt-in (chunk 1013): Ego_r.pdb / FableWin.pdb ?Overlapping@C2DBoxI@@QBE_NABV1@@Z, out of line in retail at 0x004FAB40
// (landed). Declaration only.
bool Overlapping(const C2DBoxI& other) const;                // 0x004FAB40
#endif
#ifdef FABLE_RETAIL_C2DBOXI_EXTEND
// Nested opt-in (chunk 1013, row 0x004FEC80 CWorldMap::UpdateDestroyedThemeAreas): Ego_r.pdb / FableWin.pdb ??0C2DBoxI@@QAE@ABVC2DCoordI@@0@Z
// (FableWin 0x02043330; inline in retail: the four field copies of the two corners), ?ExtendToContain@C2DBoxI@@QAEXABVC2DCoordI@@@Z
// (out of line in retail at 0x00A39E20) and ?ExtendBordersBy@C2DBoxI@@QAEXJ@Z (out of line at 0x00A39E10).
C2DBoxI(const C2DCoordI& tl,const C2DCoordI& br):TLX(tl.X),TLY(tl.Y),BRX(br.X),BRY(br.Y){}
void ExtendToContain(const C2DCoordI& pos);                  // 0x00A39E20
void ExtendBordersBy(long border);                           // 0x00A39E10
#endif
#ifdef FABLE_RETAIL_C2DBOXI_TOC2DBOXF
// Nested opt-in (chunk 1013, row 0x004FEB70 CWorldMap::HeightAreaChanged): FableWin.pdb ?ToC2DBoxF@C2DBoxI@@QBE?AVC2DBoxF@@XZ, inline in
// retail (0x004FEC20..0x004FEC43: four `fild [box+N] ; fstp` stores TLX, TLY, BRX, BRY into the argument temporary). Needs
// FABLE_RETAIL_C2DBOXF (the four-float constructor); the body follows the class.
class C2DBoxF ToC2DBoxF() const;
#endif
};
#ifdef FABLE_RETAIL_C2DBOXI_TOC2DBOXF
#include "engine/C2DBoxF.h"
inline C2DBoxF C2DBoxI::ToC2DBoxF() const { return C2DBoxF((float)TLX,(float)TLY,(float)BRX,(float)BRY); }
#endif
#endif
