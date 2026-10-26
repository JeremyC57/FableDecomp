// C3DPrimitive2::~C3DPrimitive2() -- free the primitive's data, then destroy its members in reverse order (mirrored-vertex list, partition tree, partition index buffer, the three owned buffers, the cloth primitives) and fall into the CMovableResource base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90790, length 191 bytes, plain `ret`.
// Leaf: ??1C3DPrimitive2
// Label correction: the catalog calls this ~CQuadTreeElement; it stores vtable 0x0129DF94 (RTTI C3DPrimitive2) and
// ends with the CMovableResource vtable 0x0129B860; its only caller is the deleting destructor 0x00A98430 in
// C3DPrimitive2's vtable slot 0.
// Callees: C3DPrimitive2::FreeData 0x00A901E0 (landed), free 0x00BFEA14, operator delete 0x00BFE9BC (the counted
// pointer's block), the owned objects' virtual deleting destructors (slot 0, flag 1), the cloth array's
// _Destroy_range<CCountedPointer> 0x00A96940 (landed, `__fastcall`, allocator by const reference).
// Layout: the retail 0xa0-byte C3DPrimitive2 of the landed constructor 0x00A90700 (12-byte CArrays).
// Shape: CArray's inline destructor destroys the range only for a non-trivial element type and frees the block when
// it has one; scoped_ptr's is `if (p) delete p;` through the virtual destructor; the counted pointer's releases the
// shared block and nulls both words; the base's inline destructor stores its vtable last.
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class C3DClothPrimitive;
class C3DMeshPartitionElement;
struct CRetailAllocator { CRetailAllocator() {} };
void __fastcall DestroyRange_CountedPointer_00a96940(CCountedPointer<C3DClothPrimitive>* first,
    CCountedPointer<C3DClothPrimitive>* last, const CRetailAllocator& allocator);   // 0x00A96940

inline void DestroyRange(unsigned short*, unsigned short*) {}
inline void DestroyRange(unsigned long*, unsigned long*) {}
inline void DestroyRange(CCountedPointer<C3DClothPrimitive>* first, CCountedPointer<C3DClothPrimitive>* last)
{
    DestroyRange_CountedPointer_00a96940(first, last, CRetailAllocator());
}
template<class T> class CArray { public:                        // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    ~CArray()
    {
        DestroyRange(m_begin, m_end);
        if (m_begin)
            free(m_begin);
    }
};
template<class T> struct scoped_ptr {
    T* m_p;
    ~scoped_ptr() { if (m_p) delete m_p; }
};
struct CPolymorphicResource { virtual ~CPolymorphicResource(); };
namespace N3DPrimitive { class CDeviceIndependent : public CPolymorphicResource { public:}; }
class CVertexBufferWin32 : public CPolymorphicResource { public:};
class CIndexBuffer : public CPolymorphicResource { public:};
struct CStaticBlock;
struct CAnimatedBlock;

class C3DBoundingSphere { public: long m_x, m_y, m_z, m_radiusBits; };
class C3DTextureStages { public: long DiffuseTextureStage; long BumpTextureStage; bool UseBlankTextures; };

class CMovableResource { public:                                // vtable 0x0129B860
    virtual ~CMovableResource() {}
    virtual void RelocateData();
};

class C3DPrimitive2 : public CMovableResource { public:
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
    void FreeData();                                                      // 0x00A901E0
    virtual ~C3DPrimitive2();
};

C3DPrimitive2::~C3DPrimitive2()
{
    FreeData();
}