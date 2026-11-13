// C3DPrimitive2::UploadToIndexBuffer(unsigned long) -- rebuild the primitive's index buffer: size it for every triangle list (strip
// indices, or three per triangle) times the repeating-mesh batch size, then fill it rep by rep, recording each block's start index,
// primitive count and strip flag and offsetting the indices by rep * vertex count plus the animated blocks' vertex counts.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8F9E0, 773 bytes, __fastcall `ret 4`, returns true. Ego_r.pdb / FableWin.pdb
//   ?UploadToIndexBuffer@C3DPrimitive2@@AAE_NK@Z (private; the argument is the init flags, tested `& 1`).
// Callees: operator new 0x00BFEA1A, CIndexBuffer::CIndexBuffer 0x00AC1EC0, CIndexBuffer::Initialise 0x00AC1F70 (through the inline
//   InitIndexBuffer), CIndexBuffer::Lock 0x00AC1FB0, CIndexBuffer::Unlock 0x00AC1F00, the old buffer's virtual deleting destructor
//   (slot 0, through boost::scoped_ptr::reset).
// Shape: retail first resets the buffer and runs an empty counted loop over the triangle lists (0x00A8FA22..0x00A8FA3B: a peek whose
//   result is unused), then reads the batch size and vertex count, counts the indices, computes the byte length BEFORE the second
//   `new` (ebx), and fills. The per-rep vertex base is `rep * vertex_count` (strength-reduced: [esp+20h] += vertex count, its zero
//   store inside the reps>0 guard); triangles are read as tri[j] (the +4-biased pointer walk), the strip size is read once.
// N3DPrimitive::CDeviceIndependent / CTriangleList have no shared header: modelled here from Ego_r.pdb (retail 12-byte CArrays) with
//   their FableWin.pdb inline accessors (GetVertexCount, GetTriangleListCount, PeekTriangleList(K), GetStripSize, GetTriangleCount,
//   PeekStrip, PeekTriangleList).
// Headers: overlay engine/C3DPrimitive2.h opt-ins FABLE_RETAIL_C3DPRIMITIVE2_DATA and FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER.
// Flags:
// Leaf: ?UploadToIndexBuffer@C3DPrimitive2@@AAE_NK@Z
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_DATA
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#include "engine/EPrimitiveType.h"
// Descriptive enumerator spelling (no PDB enumerator names found); same spelling as landed 0x00BBF380 / 0x00BC3E10.
enum EIndexBufferFormat { IndexFormat16 = 101 };
enum EIndexBufferPool { IndexPoolManaged = 1 };
#include "engine/CArray.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVertex2.h"
#include "engine/C3DTriangle2.h"
namespace N3DPrimitive {
class CTriangleList : public CIVCountedPointeeBase {
public:
    unsigned long GetStripSize() const { return StripIndices.size(); }
    unsigned long GetTriangleCount() const { return Triangles.size(); }
    const unsigned short* PeekStrip() const { return StripIndices.GetAsCArray(); }
    const C3DTriangle2* PeekTriangleList() const { return Triangles.GetAsCArray(); }
    CArray<C3DTriangle2> Triangles;                     // +0x08
    CArray<unsigned short> StripIndices;                // +0x14
};
class CDeviceIndependent : public CBaseClassNonCopyable {
public:
    unsigned long GetVertexCount() const { return Vertices.size(); }
    unsigned long GetTriangleListCount() const { return TriangleLists.size(); }
    const CTriangleList& PeekTriangleList(unsigned long i) const { return *TriangleLists[i]; }
    CArray<C3DVertex2> Vertices;                        // +0x04
    CArray<CIVCountedPointer<CTriangleList> > TriangleLists;   // +0x10
};
}
#include "engine/C3DPrimitive2.h"

bool C3DPrimitive2::UploadToIndexBuffer(unsigned long flags)
{
    IndexBuffer.reset(new CIndexBuffer);
    for (unsigned long l = 0; l < Data->GetTriangleListCount(); ++l)
        Data->PeekTriangleList(l);
    long reps = GetRepeatingMeshBatchSize();
    unsigned long vertex_count = Data->GetVertexCount();
    unsigned long index_count = 0;
    for (unsigned long i = 0; i < Data->GetTriangleListCount(); ++i) {
        const N3DPrimitive::CTriangleList& list = Data->PeekTriangleList(i);
        if (list.GetStripSize() > 0) index_count += list.GetStripSize();
        else index_count += list.GetTriangleCount() * 3;
    }
    unsigned long length = index_count * reps * sizeof(unsigned short);
    IndexBuffer.reset(new CIndexBuffer);
    InitIndexBuffer(*IndexBuffer, length, flags);
    unsigned short* indices = (unsigned short*)IndexBuffer->Lock(0, 0, 0);
    unsigned long pos = 0;
    for (long rep = 0; rep < reps; ++rep) {
        long offset = rep * vertex_count;
        for (unsigned long i = 0; i < Data->GetTriangleListCount(); ++i) {
            const N3DPrimitive::CTriangleList& list = Data->PeekTriangleList(i);
            N3DPrimitive::CBaseBlock* block = GetBaseBlock(i);
            block->StartIndex = pos;
            if (list.GetStripSize() > 0) {
                const unsigned short* src = list.PeekStrip();
                unsigned long n = list.GetStripSize();
                block->IsStrip = true;
                block->PrimitiveCount = n - 2;
                for (unsigned long j = 0; j < n; ++j)
                    indices[pos++] = (unsigned short)(src[j] + offset);
            } else {
                const C3DTriangle2* tri = list.PeekTriangleList();
                unsigned long n = list.GetTriangleCount();
                block->IsStrip = false;
                block->PrimitiveCount = n;
                for (unsigned long j = 0; j < n; ++j) {
                    indices[pos++] = (unsigned short)(tri[j].GetVertexIndex(0) + offset);
                    indices[pos++] = (unsigned short)(tri[j].GetVertexIndex(1) + offset);
                    indices[pos++] = (unsigned short)(tri[j].GetVertexIndex(2) + offset);
                }
            }
            if (AnimatedBlockCount)
                offset += AnimatedBlocks[i].GetVertexCount();
        }
    }
    IndexBuffer->Unlock();
    SetIndexBufferStatus((EBufferStatus)1);
    return true;
}