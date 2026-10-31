// CMatrix3x4::Invert() -- invert the libpk 3x4 matrix in place: adjugate over a clamped determinant (|det| < 1e-10 -> +/-1e-10), then the translation row -(t * R^-1)
// Grade: DIFFER(440v440) -- only the first block differs (28 diff lines): ours computes the three CSE'd cofactor temps c1, c2, c0 (stores [esp+0xc] then [esp+8]); retail c2, c1, c0 (stores [esp+0x10], [esp+4], [esp]) and loads m5 before m7 in c0's second product; determinant, clamp, adjugate and translation are byte-identical
// Leaf: ?Invert@CMatrix3x4@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D320, 440 bytes, thiscall, plain `ret`; no callers and no data references (dead libpk
//   code kept by the linker). Floats: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, epsilon 1e-10f at 0x0139C978 (.data, not const; shared
//   with the neighbouring inverse 0x00C1D1C0, which has the same clamp).
// Name: descriptive. libpk has no PDB/RTTI; the class is the landed libpk CMatrix3x4 (48-byte {Right, Up, Forward, Position},
//   the GetRotationAngles 0x00C1CC10 / RotateA 0x00C1C990 / RotateB 0x00C1C900 TU, same `rep movsd` 12-dword copy idiom).
// Shape: the engine's landed CMatrix3x4::SetInverse text (0x00AEE560 / GOTCHAS R&D final) through a local copy, plus the clamp:
//   the single-expression determinant is what makes VC7.1 CSE the cofactors into frame temps (separate float locals give a
//   58-line diff; raw float pointers and swapped operands are canonicalised the same way; unused-local k sweep 1..7 inert).
// Tried: separate cofactor locals (66/58 lines), operands swapped (58), raw `const float*` indexing (58), k unused locals 1..7
//   (no change), SetInverse text (28, this file), determinant terms reversed (worse).
// Next: SetInverse as a `__forceinline` member taking `const CMatrix3x4&` called as `SetInverse(m)` (reads through the
//   reference parameter re-key the load nodes, R&D round 3), then re-sweep k 0..7 on that shape; or the flat E11..E43 model.
#include <math.h>
extern float g_fMatrixInverseEpsilon_0139c978;

class C3DVector { public: float X, Y, Z; };

class CMatrix3x4 { public:
    C3DVector Right;        // +0x00
    C3DVector Up;           // +0x0C
    C3DVector Forward;      // +0x18
    C3DVector Position;     // +0x24
    void Invert();
};

void CMatrix3x4::Invert()
{
    CMatrix3x4 m = *this;
    float det = m.Right.X * (m.Up.Y * m.Forward.Z - m.Up.Z * m.Forward.Y) + m.Up.X * (m.Forward.Y * m.Right.Z - m.Forward.Z * m.Right.Y) + m.Forward.X * (m.Up.Z * m.Right.Y - m.Up.Y * m.Right.Z);
    if (fabs(det) < g_fMatrixInverseEpsilon_0139c978)
        det = det < 0.0f ? -g_fMatrixInverseEpsilon_0139c978 : g_fMatrixInverseEpsilon_0139c978;
    float inv = 1.0f / det;
    Right.X = (m.Up.Y * m.Forward.Z - m.Up.Z * m.Forward.Y) * inv;
    Up.X = (m.Up.Z * m.Forward.X - m.Up.X * m.Forward.Z) * inv;
    Forward.X = (m.Up.X * m.Forward.Y - m.Up.Y * m.Forward.X) * inv;
    Right.Y = (m.Forward.Y * m.Right.Z - m.Forward.Z * m.Right.Y) * inv;
    Up.Y = (m.Forward.Z * m.Right.X - m.Right.Z * m.Forward.X) * inv;
    Forward.Y = (m.Forward.X * m.Right.Y - m.Forward.Y * m.Right.X) * inv;
    Right.Z = (m.Up.Z * m.Right.Y - m.Up.Y * m.Right.Z) * inv;
    Up.Z = (m.Up.X * m.Right.Z - m.Up.Z * m.Right.X) * inv;
    Forward.Z = (m.Up.Y * m.Right.X - m.Up.X * m.Right.Y) * inv;
    Position.X = -(Right.X * m.Position.X + Up.X * m.Position.Y + Forward.X * m.Position.Z);
    Position.Y = -(Right.Y * m.Position.X + Up.Y * m.Position.Y + Forward.Y * m.Position.Z);
    Position.Z = -(Right.Z * m.Position.X + Up.Z * m.Position.Y + Forward.Z * m.Position.Z);
}
