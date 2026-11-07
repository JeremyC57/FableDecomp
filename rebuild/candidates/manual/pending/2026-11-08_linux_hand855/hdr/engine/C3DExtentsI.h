#ifndef FABLE_ENGINE_C3DEXTENTSI_H
#define FABLE_ENGINE_C3DEXTENTSI_H
// C3DExtentsI (chunk 855, row 0x00A68DE0). Ego_r.pdb: 12 bytes, the first 8 an unnamed base subobject (C2DExtentsI X, Y: the
// donor lists `_padding_` +0 / +4 and Z +8), constructors ??0C3DExtentsI@@QAE@XZ / @JJJ@Z / copy (inline in retail: the
// three arguments of CVolumeTexture::Initialise are stored member-wise, 0x00A68E78).
#include "rebuild_abi.h"
#include "engine/C2DExtentsI.h"
class C3DExtentsI : public C2DExtentsI {
public:
    C3DExtentsI() { }
    C3DExtentsI(long x, long y, long z) : C2DExtentsI(x, y), Z(z) { }
    long Z;                                                              // +0x08
};
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI) == 0xc);
#endif
