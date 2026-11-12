#ifndef FABLE_ENGINE_C2DEXTENTSI_H
#define FABLE_ENGINE_C2DEXTENTSI_H
#include "fable_compiler.h"
// Original public fields and user default constructor from Ego_r TPI.
class C2DExtentsI { public: long X,Y; C2DExtentsI() {}
#ifdef FABLE_RETAIL_C2DEXTENTSI_FORCEINLINE_CTOR
 // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): expanded in place at 0x004266EC (LOAD_PHOTO, the 512x512
 // texture extents: `mov eax,200h` stored twice) past VC7.1's caller-size inline budget, which calls a plain-inline copy there.
 FABLE_FORCEINLINE
#endif
#ifdef FABLE_RETAIL_C2DEXTENTSI_INITIALISE
 // Per-TU opt-in (chunk Q9, row 0x009F3010 CSurface::ReadOnlyLock): Ego_r.pdb ?Initialise@C2DExtentsI@@QAEXJJ@Z and the user copy constructor
 // ??0C2DExtentsI@@QAE@ABV0@@Z (Ego_r + FableWin), both inline in retail; the (x, y) constructor stores through Initialise. This is the
 // spelling of the landed sibling CSurface::Lock 0x009F33E0 (whose failure arm builds C2DExtentsI(0, 0) as `xor eax,eax ; mov edx,eax`,
 // 0x009F344E); the member-initialiser form of the default path stores the zero constant directly.
 C2DExtentsI(long x,long y){Initialise(x,y);}
 C2DExtentsI(const C2DExtentsI& o):X(o.X),Y(o.Y){}
 void Initialise(long x,long y){X=x;Y=y;}
#else
 C2DExtentsI(long x,long y):X(x),Y(y){}
#endif
};
#endif
