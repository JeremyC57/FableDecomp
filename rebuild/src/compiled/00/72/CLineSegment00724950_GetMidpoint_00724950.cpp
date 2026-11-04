// CLineSegment_00724950::GetMidpoint() const -- half of (start + end) per component, returned through the hidden pointer
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724950, length 66 bytes.
// Callees: none. Constant 0.5 (0x0122F59C). Shape: the three sums are computed first (Z parked in the 12-byte frame), then each is scaled by 0.5 and stored
// into the hidden result (X, Y from the x87 stack, Z reloaded); `ret 4` pops the hidden pointer. The same shape as CEngineInternalPrimitiveLine::GetWorldPosition
// @ 0x00A11430 (landed) with `+` for `-`. The chunk label (CEngineInternalPrimitiveParticleLine::GetWorldPosition) is propagated: the particle line's vectors sit
// at +0x48/+0x54 behind a vtable (see 0x00BC3D50), while this body reads two vectors at +0 and +0xc of a plain record, so it is modelled as a line-segment record.
#include "engine/C3DVector.h"

struct CLineSegment_00724950 {
    float m_startX, m_startY, m_startZ;            // +0x00
    float m_endX, m_endY, m_endZ;                  // +0x0c
    C3DVector GetMidpoint() const;
};

C3DVector CLineSegment_00724950::GetMidpoint() const
{
    C3DVector sum(m_endX + m_startX, m_endY + m_startY, m_endZ + m_startZ);
    return C3DVector(sum.X * 0.5f, sum.Y * 0.5f, sum.Z * 0.5f);
}