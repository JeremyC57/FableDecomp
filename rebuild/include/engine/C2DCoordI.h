#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_C2DCOORDI_NOTEQUAL
#include "fable_compiler.h"
#endif
// Ego_r TPI0xa59e, public long X/Y, native coordinate callers at0074a8a8/8e3.
#ifdef FABLE_RETAIL_C2DCOORDI_TOFLOAT
// Nested opt-in (chunk 850 pending overlay, re-applied by chunk 853; row 0x0050A650 CWorldMap::GetMapNavigationAreaInit): FableWin.pdb ?ToC2DCoordF@C2DCoordI@@QBE?AVC2DVector@@XZ
// (inline, no Ego_r / retail out-of-line copy): the float conversion of both coordinates. Needs FABLE_RETAIL_C2DVECTOR.
// (chunk 853: the class is complete before C2DVector.h is included, because C2DVector.h's FROM3D branch pulls C3DVector.h, whose inline
// ToC2DCoordI needs C2DCoordI; ToC2DCoordF is defined inline after the include.)
class C2DVector;
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    C2DVector ToC2DCoordF() const;
#ifdef FABLE_RETAIL_C2DCOORDI_NOTEQUAL
    // Retail opt-in (row 0x0064546F CDrawQuickAccessIcons::DrawEffectAtPos): FableWin.pdb ??9C2DCoordI@@QBE_NABV0@@Z (inline): X then Y compare.
#ifdef FABLE_RETAIL_C2DCOORDI_ORIGINAL_COMPARE_PASS27
    // FableWin 018867C0 / 0208EA20: != negates the two-field equality.
    inline bool operator==(const C2DCoordI& coord) const { return X == coord.X && Y == coord.Y; }
    inline bool operator!=(const C2DCoordI& coord) const { return !(*this == coord); }
#else
    FABLE_FORCEINLINE bool operator!=(const C2DCoordI& coord) const { return X!=coord.X || Y!=coord.Y; }
#endif
#endif
};
#include "engine/C2DVector.h"
inline C2DVector C2DCoordI::ToC2DCoordF() const { return C2DVector((float)X,(float)Y); }
#elif defined(FABLE_RETAIL_C2DCOORDI_COPY)
// Retail opt-in (chunk 1011, row 0x005FBAA0 CTCInventoryMap::GetRelativePosOnMiniMap): FableWin.pdb ??0C2DCoordI@@QAE@ABV0@@Z
// (0x01C85850) is a user copy constructor, inline in retail: copying a returned `const C2DCoordI&` leaves the source address
// formed (`add eax,1D24h`, 0x005FBADF) beside the two field loads. Same layout and values as the implicit copy.
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}C2DCoordI(const C2DCoordI& other):X(other.X),Y(other.Y){} };
#elif defined(FABLE_RETAIL_C2DCOORDI_INITIALISE)
// hand3c opt-in (row 0x0066CD50 CRegionMinimap::BuildRegionMinimapTGA): FableWin.pdb ?Initialise@C2DCoordI@@QAEXJJ@Z (0x021875A0; inline
// in retail: the row's position is never stored, its X / Y are the loop counters). Same layout.
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){}
    void Initialise(long x,long y) { X=x; Y=y; } };
#else
class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
#endif
FABLE_STATIC_ASSERT_AT(31, sizeof(C2DCoordI)==8);
#endif
