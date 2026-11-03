#ifndef FABLE_ENGINE_C2DBOXI_H
#define FABLE_ENGINE_C2DBOXI_H
// Original public fields and user default constructor from Ego_r TPI.
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_VALIDPOSITIONS) || defined(FABLE_RETAIL_MAP_BRUSHES)
#include "engine/C2DCoordI.h"
#endif
class C2DBoxI { public: long TLX,TLY,BRX,BRY; C2DBoxI() {}
#ifdef FABLE_RETAIL_MAP_BRUSHES
C2DCoordI GetTLPos()const{return C2DCoordI(TLX,TLY);}
C2DCoordI GetBRPos()const{return C2DCoordI(BRX,BRY);}
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
// Original user copy ctor: FableWin lib_box_i.hpp24, parameter box.
C2DBoxI(const C2DBoxI& box):TLX(box.TLX),TLY(box.TLY),BRX(box.BRX),BRY(box.BRY){}
void ClipTo(const C2DBoxI&);
bool ContainsPoint(const C2DCoordI& p)const{return p.X>=TLX && p.X<BRX && p.Y>=TLY && p.Y<BRY;}
#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
// Original Ego_r TPI0xb186; centre/radius arithmetic is native0074a685 onward.
C2DBoxI(const C2DCoordI& p,long radius){long x=p.X;long y=p.Y;Initialise(x-radius,y-radius,x+radius,y+radius);}
void Initialise(long tlx,long tly,long brx,long bry){TLX=tlx;TLY=tly;BRX=brx;BRY=bry;}
#endif
};
#endif
