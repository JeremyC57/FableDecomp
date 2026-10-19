// Grade: DIFFER(ours v retail) -- lane gap: LTCG custom register convention (matrix A in eax, point in ecx, matrix B in edx, result pointer in esi, plain `ret`); a standard-convention body cannot reproduce the argument registers
// TransformPointThroughMirroredFrame_00c1c330(const CMatrix3x4& a, const C3DVector& point, const CMatrix3x4& b) -- transform `point` by a, mirror a negative z to positive, then transform the result by b
// Retail: Fable.exe (TLC, VC7.1, LTCG) @ 0x00C1C330, 192 bytes; single caller 0x00C1C590 (the AI steering library around
//   0x00C1Cxxx, see GOTCHAS chunk 178/179: not the game's CWorld). The catalogued "CPhysicsMeshObject::
//   GetHelperPointPositionStatic(CCharString, long)" is a propagated label: the body takes no string and no long.
// Constants: 0.0f @ 0x0122DEDC, -1.0f @ 0x0122DEE0.
// Shape: each matrix is 3x3 column-major (columns +0, +0xC, +0x18) with the translation at +0x24; every component is
//   `m[2]*p.z + m[1]*p.y + m[0]*p.x + t` (z term first); `if (z < 0.0f) z *= -1.0f;` (`fcom ; test ah,5 ; jp`).
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector Column[3];
    C3DVector Translation;
};
static inline C3DVector TransformPoint(const CMatrix3x4& m, const C3DVector& p)
{
    C3DVector r;
    r.X = m.Column[2].X * p.Z + m.Column[1].X * p.Y + m.Column[0].X * p.X + m.Translation.X;
    r.Y = m.Column[2].Y * p.Z + m.Column[0].Y * p.X + m.Column[1].Y * p.Y + m.Translation.Y;
    r.Z = m.Column[2].Z * p.Z + m.Column[0].Z * p.X + m.Column[1].Z * p.Y + m.Translation.Z;
    return r;
}
C3DVector __fastcall TransformPointThroughMirroredFrame_00c1c330(const CMatrix3x4& a, const C3DVector& point, const CMatrix3x4& b)
{
    C3DVector local = TransformPoint(a, point);
    if (local.Z < 0.0f)
        local.Z *= -1.0f;
    return TransformPoint(b, local);
}
