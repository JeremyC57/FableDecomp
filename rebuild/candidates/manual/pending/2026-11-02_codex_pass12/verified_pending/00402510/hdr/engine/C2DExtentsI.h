#ifndef FABLE_ENGINE_C2DEXTENTSI_H
#define FABLE_ENGINE_C2DEXTENTSI_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DExtentsI { public: long X,Y; C2DExtentsI() {}
#if defined(FABLE_RETAIL_FONT_PIXEL_ACCESS)
 C2DExtentsI(long x,long y):X(x),Y(y){}
#endif
};
#endif
