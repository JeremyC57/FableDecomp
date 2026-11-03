#ifndef FABLE_ENGINE_C2DBOXI_H
#define FABLE_ENGINE_C2DBOXI_H
// Original public fields and user default constructor from Ego_r TPI.
#include "engine/C2DCoordI.h"
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
// Original Ego_r TPI0xb186; centre/radius arithmetic is native0074a685 onward.
C2DBoxI(const C2DCoordI& p,long radius){long x=p.X;long y=p.Y;Initialise(x-radius,y-radius,x+radius,y+radius);}
void Initialise(long tlx,long tly,long brx,long bry){TLX=tlx;TLY=tly;BRX=brx;BRY=bry;}
#ifdef FABLE_RETAIL_C2DBOXI
// Header wave 2026-11-03 (row 0x00821010 reads the brush corners): Ego_r.pdb ?GetTLPos@C2DBoxI@@QBE?AVC2DCoordI@@XZ and
// ?GetBRPos@C2DBoxI@@QBE?AVC2DCoordI@@XZ, inline (no out-of-line copy in Fable.exe).
C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}
C2DCoordI GetBRPos()const{return C2DCoordI(BRX,BRY);}
#endif
};
#endif
