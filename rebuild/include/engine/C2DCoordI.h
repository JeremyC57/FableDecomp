#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#include "rebuild_abi.h"
// Ego_r TPI0xa59e, public long X/Y, native coordinate callers at0074a8a8/8e3.
#ifdef FABLE_RETAIL_C2DCOORDI_TOFLOAT
// Nested opt-in (chunk 850 pending overlay, re-applied by chunk 853; row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): FableWin.pdb ?ToC2DCoordF@C2DCoordI@@QBE?AVC2DVector@@XZ
// (inline, no Ego_r / retail out-of-line copy): the float conversion of both coordinates. Needs FABLE_RETAIL_C2DVECTOR.
// (chunk 853: the class is complete before C2DVector.h is included, because C2DVector.h's FROM3D branch pulls C3DVector.h, whose inline
// ToC2DCoordI needs C2DCoordI; ToC2DCoordF is defined inline after the include.)
class C2DVector;
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    C2DVector ToC2DCoordF() const;
};
#include "engine/C2DVector.h"
inline C2DVector C2DCoordI::ToC2DCoordF() const { return C2DVector((float)X,(float)Y); }
#elif defined(FABLE_RETAIL_C2DCOORDI_COPY)
// Retail opt-in (chunk 1011, row 0x005FBAA0 CTCInventoryMap::GetRelativePosOnMiniMap): FableWin.pdb ??0C2DCoordI@@QAE@ABV0@@Z
// (0x01C85850) is a user copy constructor, inline in retail: copying a returned `const C2DCoordI&` leaves the source address
// formed (`add eax,1D24h`, 0x005FBADF) beside the two field loads. Same layout and values as the implicit copy.
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}C2DCoordI(const C2DCoordI& other):X(other.X),Y(other.Y){} };
#else
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif
