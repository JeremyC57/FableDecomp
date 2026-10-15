// C3DPrimitive2::C3DPrimitive2() -- vtable, then every pointer, count and status cleared, the recorded texture stages set to "none" (-1, -1, no blank textures), no recorded constants, recorded frame -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90700, length 137 bytes.
// Leaf: ??0C3DPrimitive2
// Shape: a root polymorphic class: the vtable store comes first, then the member initialisers in address
// order from one zero register (ecx; `this` moves to eax) and `or edx,-1` for the -1 words; MaterialIndex,
// AverageTextureStretch, the vertex/triangle/index counts, VertexStreamFlags and InitFlags stay uninitialised.
// The member objects (sphere, CArrays, scoped_ptrs, counted pointer, texture stages) zero through their own
// inline constructors.
// Retail C3DPrimitive2 (0xa0 bytes): the PDB layout (Ego_r 0xac) with retail's 12-byte CArrays, which puts every
// member after ClothPrimitives 4 bytes lower and every member after PartitionedMeshSourceIndexBuffer 8 bytes lower
// (as the landed 0x00A93380 / 0x00A89380 models found). Vtable 0x0129DF94 (rtti-vtables.tsv).
struct CVertexBufferWin32;
struct CIndexBuffer;
struct CStaticBlock;
struct CAnimatedBlock;
struct C3DClothPrimitive;
struct C3DMeshPartitionElement;
namespace N3DPrimitive { struct CDeviceIndependent; }

struct C3DBoundingSphere {                           // centre + radius, zeroed as dwords (landed 0x00A98100 model)
    long m_x, m_y, m_z, m_radiusBits;
    C3DBoundingSphere() : m_x(0), m_y(0), m_z(0), m_radiusBits(0) { }
};
template<class T> struct CArray {                    // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    CArray() : m_begin(0), m_end(0), m_capacity(0) { }
};
template<class T> struct scoped_ptr {
    T* m_p;
    scoped_ptr() : m_p(0) { }
};
template<class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
struct C3DTextureStages {                            // 12 bytes (PDB)
    long DiffuseTextureStage;                        // +0
    long BumpTextureStage;                           // +4
    bool UseBlankTextures;                           // +8
    C3DTextureStages() : DiffuseTextureStage(-1), BumpTextureStage(-1), UseBlankTextures(false) { }
};

struct C3DPrimitive2 {
    virtual void v00();
    long                                     MaterialIndex;               // +0x04
    long                                     RepeatingMeshReps;           // +0x08
    C3DBoundingSphere                        BoundingSphere;              // +0x0c
    float                                    AverageTextureStretch;       // +0x1c
    CArray<CCountedPointer<C3DClothPrimitive> > ClothPrimitives;          // +0x20
    scoped_ptr<N3DPrimitive::CDeviceIndependent> Data;                    // +0x2c
    scoped_ptr<CVertexBufferWin32>           VertexBuffer;                // +0x30
    scoped_ptr<CIndexBuffer>                 IndexBuffer;                 // +0x34
    unsigned long                            VertexBufferStatus;          // +0x38
    unsigned long                            IndexBufferStatus;           // +0x3c
    unsigned long                            StaticBlockCount;            // +0x40
    CStaticBlock*                            StaticBlocks;                // +0x44
    unsigned long                            AnimatedBlockCount;          // +0x48
    CAnimatedBlock*                          AnimatedBlocks;              // +0x4c
    unsigned long                            AnimatedBlocksStatus;        // +0x50
    unsigned long                            RepeatedBlocksStatus;        // +0x54
    unsigned long                            VertexCount;                 // +0x58
    unsigned long                            TriangleCount;               // +0x5c
    unsigned long                            IndexCount;                  // +0x60
    bool                                     UploadedFlag;                // +0x64
    unsigned char                            VertexStreamFlags;           // +0x65
    unsigned long                            InitFlags;                   // +0x68
    C3DTextureStages                         RecordedTextureStages;       // +0x6c
    void*                                    RecordedConstantData;        // +0x78
    long                                     RecordedFrame;               // +0x7c
    CArray<unsigned short>                   PartitionedMeshSourceIndexBuffer;   // +0x80
    CCountedPointer<C3DMeshPartitionElement> PartitionTree;               // +0x8c
    CArray<unsigned long>                    MirroredTextureVertexIndexList;     // +0x94   sizeof == 0xa0
    C3DPrimitive2();
};

C3DPrimitive2::C3DPrimitive2()
    : RepeatingMeshReps(0),
      VertexBufferStatus(0), IndexBufferStatus(0),
      StaticBlockCount(0), StaticBlocks(0), AnimatedBlockCount(0), AnimatedBlocks(0),
      AnimatedBlocksStatus(0), RepeatedBlocksStatus(0),
      UploadedFlag(false),
      RecordedConstantData(0), RecordedFrame(-1)
{
}
