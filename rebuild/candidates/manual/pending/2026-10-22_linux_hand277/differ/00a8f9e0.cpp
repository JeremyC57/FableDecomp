// Grade: DIFFER(789v773) -- register assignment (retail: indices in ebx, block pointer in esi with this spilled, parameter slot reused for the animated-block offset; ours spills the index pointer), strip/triangle source-pointer and count scheduling
// C3DPrimitive2::UploadToIndexBuffer(bool) -- build a fresh index buffer sized for every triangle list (strips or triangle triples) times the mesh repetitions, then fill it rep by rep, recording each block's start index, primitive count and strip flag and offsetting indices by the running vertex base
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8F9E0, 784 bytes (ret 4).
// Callees: operator new 0x00BFEA1A, CIndexBuffer ctor 0x00AC1EC0 / Initialise 0x00AC1F70 / Lock 0x00AC1FB0 / Unlock 0x00AC1F00,
//   the buffer's virtual deleting destructor (slot 0).
// Layout: Ego_r.pdb C3DPrimitive2 less 4 after the 12-byte retail CArray: RepeatingMeshReps +8, Data +0x2c, IndexBuffer
//   +0x34, IndexBufferStatus +0x3c, StaticBlocks +0x44, AnimatedBlockCount +0x48, AnimatedBlocks +0x4c; CStaticBlock 0x20 /
//   CAnimatedBlock 0x30 (VertexCount +0x10) over a common block head (primitive count +0, start index +4, strip flag +8);
//   N3DPrimitive::CDeviceIndependent Vertices +4 (0x20-byte C3DVertex2), TriangleLists +0x10; CTriangleList Triangles +8
//   (6-byte C3DTriangle2), StripIndices +0x14.
#include <new>
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) const { return _M_start[i]; }
    const T* GetData() const { return size() > 0 ? _M_start : (const T*)this; }
};
struct C3DVertex2 { float V[8]; };
struct C3DTriangle2 { unsigned short Index[3]; };
namespace N3DPrimitive {
struct CTriangleList {
    void* _vptr;
    long RefCount;
    CArray<C3DTriangle2> Triangles;             // +0x08
    CArray<unsigned short> StripIndices;        // +0x14
};
struct CDeviceIndependent {
    void* _vptr;
    CArray<C3DVertex2> Vertices;                // +0x04
    CArray<CTriangleList*> TriangleLists;       // +0x10
};
}
struct CIndexBuffer {
    virtual void DeleteThis(unsigned long flags);   // slot 0 (deleting destructor)
    unsigned char _body[0x10];
    CIndexBuffer();                             // 0x00AC1EC0
    bool Initialise(unsigned long length, unsigned long usage, long format, long pool, long flags);   // 0x00AC1F70
    void* Lock(unsigned long offset, unsigned long length, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                              // 0x00AC1F00
};
template <class T> struct scoped_ptr {
    T* ptr;
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        if (old)
            old->DeleteThis(1);
    }
    T* operator->() const { return ptr; }
};
struct CBlockHead {
    unsigned long PrimitiveCount;               // +0x00
    unsigned long StartIndex;                   // +0x04
    bool IsStrip;                               // +0x08
};
struct CStaticBlock : CBlockHead {
    unsigned char _pad09[0x20 - 0x09];
};
struct CAnimatedBlock : CBlockHead {
    unsigned char _pad09[0x10 - 0x09];
    unsigned long VertexCount;                  // +0x10
    unsigned char _pad14[0x30 - 0x14];
};
template <class T> inline const T& max_(const T& a, const T& b) { return a < b ? b : a; }
class C3DPrimitive2 {
public:
    void* _vptr;
    long MaterialIndex;                         // +0x04
    long RepeatingMeshReps;                     // +0x08
    unsigned char _pad0c[0x2c - 0x0c];
    N3DPrimitive::CDeviceIndependent* Data;     // +0x2c
    void* VertexBuffer;                         // +0x30
    scoped_ptr<CIndexBuffer> IndexBuffer;       // +0x34
    unsigned long VertexBufferStatus;           // +0x38
    unsigned long IndexBufferStatus;            // +0x3c
    unsigned long StaticBlockCount;             // +0x40
    CStaticBlock* StaticBlocks;                 // +0x44
    unsigned long AnimatedBlockCount;           // +0x48
    CAnimatedBlock* AnimatedBlocks;             // +0x4c
    bool UploadToIndexBuffer(bool dynamic);
};

bool C3DPrimitive2::UploadToIndexBuffer(bool dynamic)
{
    IndexBuffer.reset(new CIndexBuffer());
    for (unsigned long l = 0; l < Data->TriangleLists.size(); ++l) {
    }
    long reps = RepeatingMeshReps > 1 ? RepeatingMeshReps : 1;
    long vertsPerRep = Data->Vertices.size();
    unsigned long indexCount = 0;
    for (unsigned long i = 0; i < Data->TriangleLists.size(); ++i) {
        const N3DPrimitive::CTriangleList* list = Data->TriangleLists[i];
        if (list->StripIndices.size() > 0)
            indexCount += list->StripIndices.size();
        else
            indexCount += list->Triangles.size() * 3;
    }
    unsigned long length = indexCount * reps * 2;
    IndexBuffer.reset(new CIndexBuffer());
    unsigned long usage = 8;
    long pool = 1;
    if (dynamic) {
        usage = 0x18;
        pool = 2;
    }
    IndexBuffer->Initialise(length, usage, 0x65, pool, 4);
    unsigned short* indices = (unsigned short*)IndexBuffer->Lock(0, 0, 0);
    unsigned long pos = 0;
    long repBase = 0;
    for (long rep = 0; rep < reps; ++rep) {
        long offset = repBase;
        for (unsigned long i = 0; i < Data->TriangleLists.size(); ++i) {
            const N3DPrimitive::CTriangleList* list = Data->TriangleLists[i];
            CBlockHead* block = AnimatedBlockCount ? (CBlockHead*)&AnimatedBlocks[i] : (CBlockHead*)&StaticBlocks[i];
            block->StartIndex = pos;
            if (list->StripIndices.size() > 0) {
                const unsigned short* src = list->StripIndices.GetData();
                unsigned long n = list->StripIndices.size();
                block->IsStrip = true;
                block->PrimitiveCount = n - 2;
                for (unsigned long j = 0; j < n; ++j)
                    indices[pos++] = (unsigned short)(src[j] + offset);
            } else {
                const C3DTriangle2* tri = list->Triangles.GetData();
                unsigned long n = list->Triangles.size();
                block->IsStrip = false;
                block->PrimitiveCount = n;
                for (unsigned long j = 0; j < n; ++j, ++tri) {
                    indices[pos++] = (unsigned short)(tri->Index[0] + offset);
                    indices[pos++] = (unsigned short)(tri->Index[1] + offset);
                    indices[pos++] = (unsigned short)(tri->Index[2] + offset);
                }
            }
            if (AnimatedBlockCount)
                offset += AnimatedBlocks[i].VertexCount;
        }
        repBase += vertsPerRep;
    }
    IndexBuffer->Unlock();
    IndexBufferStatus |= 1;
    return true;
}
