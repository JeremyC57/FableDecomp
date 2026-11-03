#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#include "rebuild_abi.h"
// Ego_r TPI0xa59e, public long X/Y, native coordinate callers at0074a8a8/8e3.
class C2DCoordI { public: long X,Y;C2DCoordI(){}
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
// Original user copy ctor: FableWin lib_coord.hpp39, named p parameter.
C2DCoordI(const C2DCoordI& p):X(p.X),Y(p.Y){}
#endif
C2DCoordI(long x,long y):X(x),Y(y){} };
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif
