// CEngineInternalAttachedPrimitive::GetChildInverseTransform() -- true once the cached inverse at +0xa8 is valid; otherwise compute it for a static-mesh child (type 1) and remember whether that succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B39900, length 47 bytes.
// Callees: GetChildInverseTransformStaticMesh(CMatrix3x4*) @ 0x00B393D0 (on `this`, the cached matrix at +0xa8). Shape: the early `mov al,1 ; ret`, then
// `xor al,al ; dec ecx ; jne` -- the child type is dispatched as a one-case switch (case 1), the result byte is stored to the valid flag at +0xd8 either
// way. No PDB layout for the attached primitive; member names are descriptive.
class CMatrix3x4 { public:
    float m_rows[12];
};

class CEngineInternalAttachedPrimitive { public:
    unsigned char _pad[0x7c];
    long          m_childType;                     // +0x7c: 1 = static mesh
    unsigned char _pad80[0x28];
    CMatrix3x4    m_childInverseTransform;         // +0xa8
    bool          m_childInverseValid;             // +0xd8
    bool GetChildInverseTransformStaticMesh(CMatrix3x4* inverse);   // 0x00B393D0
    bool GetChildInverseTransform();
};

bool CEngineInternalAttachedPrimitive::GetChildInverseTransform()
{
    if (m_childInverseValid)
        return true;
    bool valid = false;
    switch (m_childType) {
    case 1:
        valid = GetChildInverseTransformStaticMesh(&m_childInverseTransform);
        break;
    }
    m_childInverseValid = valid;
    return valid;
}