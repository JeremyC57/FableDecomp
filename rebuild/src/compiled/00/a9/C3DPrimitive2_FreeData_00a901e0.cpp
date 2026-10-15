// C3DPrimitive2::FreeData() -- release the primitive's device data: empty the partitioned-mesh index buffer, delete the device-independent data, vertex buffer and index buffer (zeroing their statuses), destroy the animated blocks and forget the block arrays
// Leaf: FreeData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A901E0, length 126 bytes.
// Label correction: catalogued as CInputManagerDX::~CInputManagerDX; it stores no vtable, and its callers are
// C3DPrimitive2's destructor 0x00A90790 (right after storing vtable 0x0129DF94 = C3DPrimitive2) and 0x00A91640.
// The offsets fit Ego_r.pdb C3DPrimitive2 with the retail 12-byte CArray (every member after ClothPrimitives 4 low).
// Callees: memmove @ 0x00BFEAE6 (the inlined POD copy of `erase(begin(), end())`), the three scoped_ptrs' virtual
// deleting destructors (slot 0, flag 1), the animated-block range destroy @ 0x00A96810 (`__fastcall`, blocks in ecx,
// count in edx).
// Shape: the erase copies with the early-out helper (`if (first == last) return result;` -> `cmp eax,eax ; jne`, the
// member read once), the byte count added to memmove's result; each scoped_ptr reset clears the member before the null
// test; one zero register (edi) for every store.
#include <string.h>
template <class T> struct CArray {                 // retail CArray: first / last / end
    T* m_first; T* m_last; T* m_end;
    static T* CopyTrivial(const T* first, const T* last, T* result)
    {
        if (first == last)
            return result;
        size_t n = (const char*)last - (const char*)first;
        return (T*)((char*)memmove(result, first, n) + n);
    }
    T* erase(T* first, T* last)
    {
        T* i = CopyTrivial(last, m_last, first);
        m_last = i;
        return first;
    }
    void clear() { erase(m_first, m_last); }
};
template <class T> struct scoped_ptr {
    T* m_p;
    void reset()
    {
        T* old = m_p;
        m_p = 0;
        if (old)
            delete old;
    }
};
struct CPolymorphicResource { virtual ~CPolymorphicResource(); };
struct CDeviceIndependent : CPolymorphicResource {};
struct CVertexBufferWin32 : CPolymorphicResource {};
struct CIndexBuffer : CPolymorphicResource {};
struct CStaticBlock;
struct CAnimatedBlock;
struct CCountedPointerAny { void* m_object; void* m_block; };
void __fastcall DestroyAnimatedBlocks(CAnimatedBlock* blocks, unsigned long count);   // 0x00A96810

struct C3DPrimitive2 {
    virtual void v00();
    long                             MaterialIndex;        // +0x04
    long                             RepeatingMeshReps;    // +0x08
    float                            BoundingSphere[4];    // +0x0c
    float                            AverageTextureStretch;   // +0x1c
    CArray<CCountedPointerAny>       ClothPrimitives;      // +0x20
    scoped_ptr<CDeviceIndependent>   Data;                 // +0x2c
    scoped_ptr<CVertexBufferWin32>   VertexBuffer;         // +0x30
    scoped_ptr<CIndexBuffer>         IndexBuffer;          // +0x34
    unsigned long                    VertexBufferStatus;   // +0x38
    unsigned long                    IndexBufferStatus;    // +0x3c
    unsigned long                    StaticBlockCount;     // +0x40
    CStaticBlock*                    StaticBlocks;         // +0x44
    unsigned long                    AnimatedBlockCount;   // +0x48
    CAnimatedBlock*                  AnimatedBlocks;       // +0x4c
    unsigned long                    AnimatedBlocksStatus; // +0x50
    unsigned char                    _pad54[0x2c];
    CArray<unsigned short>           PartitionedMeshSourceIndexBuffer;   // +0x80
    void FreeData();
};

void C3DPrimitive2::FreeData()
{
    PartitionedMeshSourceIndexBuffer.clear();
    Data.reset();
    VertexBuffer.reset();
    VertexBufferStatus = 0;
    IndexBuffer.reset();
    IndexBufferStatus = 0;
    DestroyAnimatedBlocks(AnimatedBlocks, AnimatedBlockCount);
    StaticBlocks = 0;
    AnimatedBlocks = 0;
    AnimatedBlocksStatus = 0;
}