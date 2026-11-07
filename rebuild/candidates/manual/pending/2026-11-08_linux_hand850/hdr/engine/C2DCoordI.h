#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#include "rebuild_abi.h"
// Ego_r TPI0xa59e, public long X/Y, native coordinate callers at0074a8a8/8e3.
#ifdef FABLE_RETAIL_C2DCOORDI_TOFLOAT
// Nested opt-in (chunk 850, row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): FableWin.pdb ?ToC2DCoordF@C2DCoordI@@QBE?AVC2DVector@@XZ
// (inline, no Ego_r / retail out-of-line copy): the float conversion of both coordinates. Needs FABLE_RETAIL_C2DVECTOR.
#include "engine/C2DVector.h"
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    C2DVector ToC2DCoordF() const { return C2DVector((float)X,(float)Y); }
};
#else
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif
