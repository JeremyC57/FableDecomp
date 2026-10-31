// CMatrix4x4::SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4& source) -- invert a uniform-scale * rotation + translation matrix: the transposed 3x3 divided by the squared scale, the translation rotated back and negated, bottom row (0, 0, 0, 1)
// Grade: DIFFER(211v211) -- scheduling only: the bottom-row zero register `xor eax,eax` sits right after the last `fld [eax+0Ch]` in retail, ours after the row-3 `fchs` (3 slots later); every x87 operand order now matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D000, length 211 bytes (`ret 4`).
// Callees: none; 1.0f @ 0x0122DED8.
// Layout: CMatrix4x4 is 16 floats, row r at +0x10*r (PDB: a union, 64 bytes).
// Shape (chunk 631): the old translation x is read through a raw float pointer `s[3]` (s = &source.m[0][0]); that re-keys VC7.1 load-node identity and gives retail the source-first order in rows 1 and 3 while row 2 keeps m-first (whole-translation pointer form flips row 2; member form leaves rows 1/3 wrong).
// Shape: the squared scale is the length squared of the first column (through a C3DVector temporary); each element of the 3x3 is
// `inverse * source`; the new translation is the negated dot product of each new row with the old translation, written as `d +=` chains
// in retail's term order (a single sum is reordered by operand address); the bottom-row zeros share one register.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CMatrix4x4 {
    float m[4][4];
    float SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4& source);
};

float CMatrix4x4::SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4& source)
{
    float inverseScaleSquared = 1.0f / C3DVector(source.m[0][0], source.m[1][0], source.m[2][0]).GetLengthSquared();
    m[0][0] = inverseScaleSquared * source.m[0][0];
    m[0][1] = inverseScaleSquared * source.m[1][0];
    m[0][2] = inverseScaleSquared * source.m[2][0];
    m[1][0] = inverseScaleSquared * source.m[0][1];
    m[1][1] = inverseScaleSquared * source.m[1][1];
    m[1][2] = inverseScaleSquared * source.m[2][1];
    m[2][0] = inverseScaleSquared * source.m[0][2];
    m[2][1] = inverseScaleSquared * source.m[1][2];
    m[2][2] = inverseScaleSquared * source.m[2][2];
    const float* s = &source.m[0][0];
    float d = m[0][2] * source.m[2][3];
    d += s[3] * m[0][0];
    d += m[0][1] * source.m[1][3];
    m[0][3] = -d;
    d = m[1][1] * source.m[1][3];
    d += m[1][2] * source.m[2][3];
    d += s[3] * m[1][0];
    m[1][3] = -d;
    d = m[2][1] * source.m[1][3];
    d += m[2][2] * source.m[2][3];
    d += s[3] * m[2][0];
    m[2][3] = -d;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1.0f;
    return inverseScaleSquared;
}
