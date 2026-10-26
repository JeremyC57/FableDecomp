// CEngineInternalAttachedPrimitive::RedoMatrixConversion(CMatrix3x4* matrix) -- rescale a 3x4 matrix in place: the third row (elements 6..8) by 100.0f, the first two rows (elements 0..5) by -100.0f
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B38DB0, length 113 bytes (+ int3 padding).
// Callees: none. Float pool: 0x01230A08 = 100.0f, 0x01275E8C = -100.0f.
// Shape: `this` is never read (thiscall member, `ret 4`); every element is `fld [m+i] ; fmul [pool] ; fstp [m+i]`
// -- an in-place `m[i] *= literal` (a literal multiplier is loaded after the operand). The third row is
// rescaled before the first two, which fixes the source statement order. The twin at 0x00B38E30 applies the
// inverse factors (0.01f / -0.01f). No PDB layout for the attached primitive is needed (`this` unused);
// the matrix is modelled as twelve floats.
class CMatrix3x4 { public:
    float m_elements[12];
};

class CEngineInternalAttachedPrimitive { public:
    void RedoMatrixConversion(CMatrix3x4* matrix);
};

void CEngineInternalAttachedPrimitive::RedoMatrixConversion(CMatrix3x4* matrix)
{
    matrix->m_elements[6] *= 100.0f;
    matrix->m_elements[7] *= 100.0f;
    matrix->m_elements[8] *= 100.0f;
    matrix->m_elements[0] *= -100.0f;
    matrix->m_elements[1] *= -100.0f;
    matrix->m_elements[2] *= -100.0f;
    matrix->m_elements[3] *= -100.0f;
    matrix->m_elements[4] *= -100.0f;
    matrix->m_elements[5] *= -100.0f;
}