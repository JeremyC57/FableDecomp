#ifndef FABLE_ENGINE_C2DBOXI_H
#define FABLE_ENGINE_C2DBOXI_H
// Original public fields and user default constructor from Ego_r TPI.
#include "engine/C2DCoordI.h"
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
// Original Ego_r TPI0xb186; centre/radius arithmetic is native0074a685 onward.
C2DBoxI(const C2DCoordI& p,long radius){long x=p.X;long y=p.Y;Initialise(x-radius,y-radius,x+radius,y+radius);}
void Initialise(long tlx,long tly,long brx,long bry){TLX=tlx;TLY=tly;BRX=brx;BRY=bry;}
#ifdef FABLE_RETAIL_C2DBOXI
// Retail opt-in (2026-11-03, chunk 719 row 0x00509A20, agent719c): Ego_r.pdb / FableWin.pdb inline accessors (no out-of-line
// copy bound in Fable.exe; 0x00509A20 expands ContainsPoint as TLX, BRX, TLY, BRY compares). No layout change.
long GetWidth() const { return BRX-TLX; }
long GetHeight() const { return BRY-TLY; }
bool ContainsPoint(const C2DCoordI& p) const { return p.X>=TLX && p.X<BRX && p.Y>=TLY && p.Y<BRY; }
#endif
};
#endif
