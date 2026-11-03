#ifndef FABLE_ENGINE_C2DEXTENTSI_H
#define FABLE_ENGINE_C2DEXTENTSI_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DExtentsI { public: long X,Y; C2DExtentsI() {}
 C2DExtentsI(long x,long y):X(x),Y(y){}
#ifdef FABLE_RETAIL_C2DEXTENTSI
 // Retail opt-in (2026-11-03, chunk 719 row 0x0040CEC0): FableWin.pdb ??0C2DExtentsI@@QAE@ABV0@@Z (the user copy ctor: a
 // by-value C2DExtentsI argument is built in place, sub esp,8 / mov eax,esp / two stores) and ?Initialise@C2DExtentsI@@QAEXJJ@Z.
 C2DExtentsI(const C2DExtentsI& other):X(other.X),Y(other.Y){}
 void Initialise(long x,long y){X=x;Y=y;}
#endif
};
#endif
