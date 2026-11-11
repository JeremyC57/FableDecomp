#ifndef FABLE_ENGINE_NBBBMATHS_CLINEARSPLINE_H
#define FABLE_ENGINE_NBBBMATHS_CLINEARSPLINE_H
// New shared header (w22 agent2022, row 0x007F26B0 CTCLightningOrb::Initialise): NBBBMaths::CLinearSpline (class, no vtable).
// Ego_r.pdb layout: ControlPoints vector<C3DVector> +0x00, ProcessedPoints vector<C3DVector> (Ego_r +0x10, retail +0x0C: 12-byte STLport
// vectors), sizeof 0x18 in retail (Initialise 0x007F2A02 `push 18h ; call operator new` then the inline ??0CLinearSpline@NBBBMaths@@QAE@XZ
// zeroes the six vector words). Methods: Ego_r.pdb / FableWin.pdb, out of line in retail in that donor order: GetLength() const 0x00A57EF0,
// GetPointWithParam(float) const 0x00A57F70, AddControlPoint(const C3DVector&) 0x00A580F0, ProcessPoints(long) 0x00A58130 (`lea edi,
// [ecx+0Ch]` = ProcessedPoints). The landed rows 0x00A580F0 / 0x00A57EF0 / 0x00A57F70 model it locally.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
namespace NBBBMaths {
class CLinearSpline {
public:
    float GetLength() const;                                     // 0x00A57EF0
    C3DVector GetPointWithParam(float param) const;              // 0x00A57F70
    void AddControlPoint(const C3DVector& point);                // 0x00A580F0
    void ProcessPoints(long subdivisions);                       // 0x00A58130 (parameter name descriptive)
    std::vector<C3DVector> ControlPoints;                        // +0x00
    std::vector<C3DVector> ProcessedPoints;                      // +0x0c
};
}
FABLE_STATIC_ASSERT(sizeof(NBBBMaths::CLinearSpline) == 0x18);
#endif
