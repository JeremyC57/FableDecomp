#ifndef FABLE_ENGINE_CSPHERE_H
#define FABLE_ENGINE_CSPHERE_H
// New header (chunk Q14, row 0x00A58790): NBBBMaths::CSphere, Ego_r.pdb (struct_layouts_egor.tsv `@STRUCT CSphere 16`):
// Radius (float) +0x00, Base (C3DVector) +0x04, sizeof 0x10. Namespace and members from both PDBs' mangled names:
// ??0CSphere@NBBBMaths@@QAE@ABVC3DVector@@M@Z (FableWin.pdb) and
// ?GetFirstIntersectionWithFiniteLine@CSphere@NBBBMaths@@QBE_NABVC3DVector@@0AAV3@1@Z (Ego_r.pdb 0x00BB0A10, FableWin.pdb;
// retail 0x00A58790, out of line). Declarations only.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
namespace NBBBMaths {
class CSphere {
public:
    CSphere(const C3DVector& base, float radius);
    bool GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector& point, C3DVector& normal) const;   // 0x00A58790
    float     Radius;                                                    // +0x00
    C3DVector Base;                                                      // +0x04
};
}
FABLE_STATIC_ASSERT(sizeof(NBBBMaths::CSphere) == 0x10);
#endif
