// CMatrix3x4::Invert() -- invert the libpk 3x4 matrix in place: adjugate over a clamped determinant (|det| < 1e-10 -> +/-1e-10), then the translation row -(t * R^-1)
// Leaf: ?Invert@CMatrix3x4@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D320, 440 bytes, thiscall, plain `ret`; no callers and no data references (dead libpk
//   code kept by the linker). Floats: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, epsilon 1e-10f at 0x0139C978 (.data, not const; shared
//   with the neighbouring __fastcall inverse 0x00C1D1C0, which has the same clamp).
// Name: descriptive. libpk has no PDB/RTTI; the class is libpk's own CMatrix3x4 (the GetRotationAngles 0x00C1CC10 / RotateA
//   0x00C1C990 / PreMultiply 0x00C1CDA0 TU, out of line in this library), 48 bytes laid out as the engine's flat E11..E43
//   struct arm (engine/CMatrix3x4.h; not included: the engine header models the game's inline-only class).
// Shape: a copy of *this (`rep movsd`), the determinant by cofactors of the first column with E11's minor written
//   `E22*E33 - E32*E23`, the clamp, then the engine's SetInverse adjugate / translation text. VC7.1 CSEs the three minors into
//   frame temps in the order retail has only for the flat E11..E43 members with that spelling of the first minor; through
//   C3DVector rows (Right/Up/Forward/Position) the same expression gives the minors in another order or swaps the first
//   product's loads (4..28 diff lines; 64 operand-order and 192 term-order spellings tried on that model, md 4 at best).
#include <math.h>
extern float g_fMatrixInverseEpsilon;   // 0x0139C978 (1e-10f)

class CMatrix3x4 { public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    void Invert();
};

void CMatrix3x4::Invert()
{
    CMatrix3x4 m = *this;
    float det = m.E11 * (m.E22 * m.E33 - m.E32 * m.E23) + m.E21 * (m.E13 * m.E32 - m.E12 * m.E33) + m.E31 * (m.E12 * m.E23 - m.E13 * m.E22);
    if (fabs(det) < g_fMatrixInverseEpsilon)
        det = det < 0.0f ? -g_fMatrixInverseEpsilon : g_fMatrixInverseEpsilon;
    float inv = 1.0f / det;
    E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
    E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
    E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
    E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
    E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
    E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
    E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
    E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
    E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
}