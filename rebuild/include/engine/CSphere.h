#ifndef FABLE_ENGINE_CSPHERE_H
#define FABLE_ENGINE_CSPHERE_H
// New header (chunk Q14, row 0x00A58790): NBBBMaths::CSphere, Ego_r.pdb (struct_layouts_egor.tsv `@STRUCT CSphere 16`):
// Radius (float) +0x00, Base (C3DVector) +0x04, sizeof 0x10. Namespace and members from both PDBs' mangled names:
// ??0CSphere@NBBBMaths@@QAE@ABVC3DVector@@M@Z (FableWin.pdb) and
// ?GetFirstIntersectionWithFiniteLine@CSphere@NBBBMaths@@QBE_NABVC3DVector@@0AAV3@1@Z (Ego_r.pdb 0x00BB0A10, FableWin.pdb;
// retail 0x00A58790, out of line). Declarations only.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#ifdef FABLE_RETAIL_CSPHERE_TRIANGLE
#include "stlport/fable_containers.h"
#endif
namespace NBBBMaths {
#ifdef FABLE_RETAIL_CPHYSICSMESH_SWEEP_TRIANGLE_PASS30
float FABLE_FASTCALL GetSquaredDistanceBetweenSphereTriangle(const C3DVector& position,float radius,
    const C3DVector& a,const C3DVector& b_a,const C3DVector& c_a);
#endif
class CSphere {
public:
    CSphere(const C3DVector& base, float radius);
    bool GetFirstIntersectionWithFiniteLine(const C3DVector& start, const C3DVector& end, C3DVector& point, C3DVector& normal) const;   // 0x00A58790
#ifdef FABLE_RETAIL_CSPHERE_TRIANGLE
    // Retail opt-in (p25F, row 0x00A58B50): Ego_r.pdb ?GetFirstIntersectionWithTriangle@CSphere@NBBBMaths@@QBE_NABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@1@Z.
    bool GetFirstIntersectionWithTriangle(const std::vector<C3DVector>& triangle, C3DVector& intersection_pos, C3DVector& outward_normal) const;   // 0x00A58B50
#endif
    float     Radius;                                                    // +0x00
    C3DVector Base;                                                      // +0x04
};
}
FABLE_STATIC_ASSERT_AT(19, sizeof(NBBBMaths::CSphere) == 0x10);
#endif
