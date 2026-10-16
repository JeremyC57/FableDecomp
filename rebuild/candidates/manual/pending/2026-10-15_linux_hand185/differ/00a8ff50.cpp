// Grade: DIFFER(217v231) -- retail copies position/normal X,Y through the FPU (fld/fxch/fstp) and Z as a dword via a 0x1c-byte frame temporary, with the output pointer biased to +8; ours copies all as dwords | tried: user copy ctor getters, 3-float ctor getters, POD by-value getters | next: getter building a C3DVector from three floats with ONLY a 3-float ctor plus a named vertex temp (C3DVertex2 copy) -- graphics row
// N3DPrimitive::CAnimatedBlock::ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, CDeviceIndependent& mesh) -- fill the blended vertex buffer: for every block vertex copy position/normal/uv from the mesh vertex it indexes, then its bone indices (x3, palette slots) and weights, zero-padding to four
// Leaf: ConvertToVBFormatNonBump
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8FF50, length 231 bytes (+9 int3), ret 8. No callees.
// Ego_r.pdb ?ConvertToVBFormatNonBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertexNormBlend4Tex1@@AAVCDeviceIndependent@2@@Z.
// Owner note (graphics/rendering row): true class N3DPrimitive::CAnimatedBlock (non-polymorphic here: no vtable use);
// retail offsets: BonesPerVertex +0x14 (ushort), Data +0x2c (CIVCountedPointer<CAnimatedBlock::CDeviceIndependent>:
// VertexList CArray<ulong> +0x08 {data, end}, BoneWeights CArray<C3DIndexedBoneWeight> +0x14, 8-byte weights: bone byte +0,
// weight byte +4). Mesh N3DPrimitive::CDeviceIndependent: Vertices CArray<C3DVertex2> data +0x04 (32-byte vertices:
// position +0, normal +0xc, uv +0x18). Output CTVertexNormBlend4Tex1 (0x28): position +0, bone indices +0x0c, weights +0x10,
// normal +0x14, uv +0x20.
// Shape: position and normal are by-value C3DVector getters (x/y through the FPU, z as a dword copy through the frame
// temporary); the uv is a plain struct copy. The bone loop re-reads BonesPerVertex each pass, the padding loop continues
// from the same counter; the bone index byte is multiplied by 3 as a byte (`imul bl`).
struct C3DVector {
    float X, Y, Z;
};
struct C2DVector { float X, Y; };
struct C3DVertex2 {                                // 32 bytes
    C3DVector Position;                            // +0x00
    C3DVector Normal;                              // +0x0c
    C2DVector UV;                                  // +0x18
    C3DVector GetPosition() const { return Position; }
    C3DVector GetNormal() const { return Normal; }
};
struct CTVertexNormBlend4Tex1 {                    // 0x28 bytes
    C3DVector Position;                            // +0x00
    unsigned char BoneIndices[4];                  // +0x0c
    unsigned char BoneWeights[4];                  // +0x10
    C3DVector Normal;                              // +0x14
    C2DVector UV;                                  // +0x20
};
struct C3DIndexedBoneWeight {                      // 8 bytes
    unsigned char Bone;                            // +0x00
    unsigned char _pad1[3];
    unsigned char Weight;                          // +0x04
    unsigned char _pad5[3];
};
template <class T> struct CArray {                 // retail 12 bytes
    T* Data;                                       // +0x00
    T* End;                                        // +0x04
    T* Capacity;                                   // +0x08
    unsigned long Size() const { return End - Data; }
    T& operator[](unsigned long i) const { return Data[i]; }
};

namespace N3DPrimitive {
struct CDeviceIndependent {                        // the mesh's device-independent data
    long _f0;                                      // +0x00
    CArray<C3DVertex2> Vertices;                   // +0x04
};

struct CAnimatedBlock {
    struct CDeviceIndependent {
        long _f0;                                  // +0x00
        long IVRefCount;                           // +0x04
        CArray<unsigned long> VertexList;          // +0x08
        CArray<C3DIndexedBoneWeight> BoneWeights;  // +0x14
    };
    unsigned char _pad0[0xc];                      // +0x00
    unsigned long BoneConstantsFixupOffset;        // +0x0c
    unsigned long VertexCount;                     // +0x10
    unsigned short BonesPerVertex;                 // +0x14
    bool PalettedFlag;                             // +0x16
    unsigned char GroupCount;                      // +0x17
    unsigned char Groups[0x14];                    // +0x18
    CDeviceIndependent* Data;                      // +0x2c CIVCountedPointer
    void ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, N3DPrimitive::CDeviceIndependent& mesh);
};

void CAnimatedBlock::ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, N3DPrimitive::CDeviceIndependent& mesh)
{
    unsigned long count = Data->VertexList.Size();
    for (unsigned long i = 0; i < count; ++i) {
        const C3DVertex2& vertex = mesh.Vertices[Data->VertexList[i]];
        out[i].Position = vertex.GetPosition();
        out[i].Normal = vertex.GetNormal();
        out[i].UV = vertex.UV;
        unsigned long j;
        for (j = 0; j < BonesPerVertex; ++j) {
            const C3DIndexedBoneWeight& weight = Data->BoneWeights[i * BonesPerVertex + j];
            out[i].BoneIndices[j] = weight.Bone * 3;
            out[i].BoneWeights[j] = weight.Weight;
        }
        for (; j < 4; ++j) {
            out[i].BoneIndices[j] = 0;
            out[i].BoneWeights[j] = 0;
        }
    }
}
}
