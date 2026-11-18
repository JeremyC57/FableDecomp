#ifndef FABLE_ENGINE_C3DEXTENTSI_H
#define FABLE_ENGINE_C3DEXTENTSI_H
// New header (e17, row 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading local `dimensions`): Ego_d.pdb class record 0xBCDD
// (sizeof 12; public base C2DExtentsI, member `long Z` at +8; three constructors (const C3DExtentsI&) / (long x, long y, long z) / ()
// and Initialise(long, long, long)); Ego_r.pdb struct_layouts_egor.tsv C3DExtentsI 12 bytes, Z at +8; FableWin.pdb ??0C3DExtentsI@@QAE@JJJ@Z,
// ??0C3DExtentsI@@QAE@XZ, ??0C3DExtentsI@@QAE@ABV0@@Z. The (x, y, z) constructor's Ego_d debug length 41 = a call of the base (x, y)
// constructor plus the Z store (parameter names x / y / z from Ego_d).
#include "fable_compiler.h"
#include "engine/C2DExtentsI.h"
class C3DExtentsI : public C2DExtentsI {
public:
    long Z;
    C3DExtentsI() {}
    C3DExtentsI(long x, long y, long z) : C2DExtentsI(x, y), Z(z) {}
    C3DExtentsI(const C3DExtentsI& p) : C2DExtentsI(p), Z(p.Z) {}
    void Initialise(long x, long y, long z) { X = x; Y = y; Z = z; }
};
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI) == 12);
#endif
