// CMatrix3x4::IsIdentity() const -- true when every row (and the translation) is within the squared tolerance of the identity matrix
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A560A0, length 209 bytes (`ret`).
// Callees: none; constants: the tolerance extern float @ 0x0129BA3C (0.0001f), 1.0f @ 0x0122DED8.
// Layout: four C3DVector rows (landed CMatrix3x4 0x00A56530 model): rows +0x00/+0x0C/+0x18, translation +0x24.
// Shape: every test squares the tolerance (VC7.1 computes it once into the `push ecx` frame word); each row minus its identity row
// (the zero subtractions fold, the translation included) is compared by squared length; the four tests are one `&&` chain
// guarding `return true` (the shared `return false` exit after it).
#include "engine/C3DVector.h"
extern const float DELTA_NOTIONAL_ZERO;            // 0x0129BA3C (0.0001f)
class CMatrix3x4 { public:
    C3DVector m_row0;                           // +0x00
    C3DVector m_row1;                           // +0x0c
    C3DVector m_row2;                           // +0x18
    C3DVector m_translation;                    // +0x24
    bool IsIdentity() const;
};

bool CMatrix3x4::IsIdentity() const
{
    if (!((m_row0 - C3DVector(1.0f, 0.0f, 0.0f)).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) &&
        !((m_row1 - C3DVector(0.0f, 1.0f, 0.0f)).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) &&
        !((m_row2 - C3DVector(0.0f, 0.0f, 1.0f)).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) &&
        !((m_translation - C3DVector(0.0f, 0.0f, 0.0f)).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return true;
    return false;
}