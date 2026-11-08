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
 C2DExtentsI(long x,long y):X(x),Y(y){}
};
#endif
