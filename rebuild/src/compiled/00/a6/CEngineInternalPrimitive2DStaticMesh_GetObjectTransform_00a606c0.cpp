// CEngineInternalPrimitive2DStaticMesh::GetObjectTransform() const -- an identity 2D transform (32 bytes) built by the out-of-line constructor @ 0x009E9AB0 in a local and copied into the hidden result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A606C0, length 37 bytes.
// Callees: the transform's default constructor @ 0x009E9AB0 (on the 32-byte local; manifest "CDateAndTime::CDateAndTime" is propagated). Shape: VC7.1 has no
// named-return-value optimisation, so `CTransform2D t; return t;` constructs the local and copies it with `rep movsd` (8 dwords) into the hidden pointer;
// `ret 4` pops that pointer, eax still holds it. The second copy of this body (0x00A606C0 / 0x00A60860) is the same method in another translation unit.
struct CTransform2D {
    float m_values[8];                             // 32 bytes
    CTransform2D();                                // 0x009E9AB0
};

struct CEngineInternalPrimitive2DStaticMesh {
    CTransform2D GetObjectTransform() const;
};

CTransform2D CEngineInternalPrimitive2DStaticMesh::GetObjectTransform() const
{
    CTransform2D transform;
    return transform;
}