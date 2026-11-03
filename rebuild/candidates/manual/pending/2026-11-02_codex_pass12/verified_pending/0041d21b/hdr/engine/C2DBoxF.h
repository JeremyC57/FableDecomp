#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H
// Original public fields and user default constructor from Ego_r TPI.
#ifdef FABLE_RETAIL_STATBOX
#include "engine/C2DVector.h"
#ifdef FABLE_RETAIL_SUBTITLE_BOX
#include "engine/C2DExtentsF.h"
#endif
#endif
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
#ifdef FABLE_RETAIL_STATBOX
#ifdef FABLE_RETAIL_SUBTITLE_BOX
 C2DBoxF(const C2DVector& tlpos,const C2DExtentsF& range){Initialise(tlpos,range);}
 void Initialise(const C2DVector& tlpos,const C2DExtentsF& range){TLX=tlpos.X;TLY=tlpos.Y;BRX=TLX+range.X;BRY=TLY+range.Y;}
 C2DVector GetTLPos()const{return C2DVector(TLX,TLY);}
 float GetWidth()const{return BRX-TLX;}float GetHeight()const{return BRY-TLY;}
#endif
#endif
#if defined(FABLE_RETAIL_STATBOX) || defined(FABLE_RETAIL_NAV_OUTPUT)
 C2DBoxF(const C2DBoxF& box):TLX(box.TLX),TLY(box.TLY),BRX(box.BRX),BRY(box.BRY){}
#endif
#ifdef FABLE_RETAIL_STATBOX
 C2DBoxF(float tlx,float tly,float brx,float bry):TLX(tlx),TLY(tly),BRX(brx),BRY(bry){}
 C2DBoxF(const C2DVector& tl,const C2DVector& br):TLX(tl.X),TLY(tl.Y),BRX(br.X),BRY(br.Y){}
#endif
};
#endif
