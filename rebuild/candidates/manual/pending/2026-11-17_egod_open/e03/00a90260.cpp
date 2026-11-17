// C3DPrimitive2::CreateDegenerateTriangleIndexBuffer() -- copy the index runs of the degenerate-triangle static blocks into a new index buffer (rebasing them), clear the other blocks, swap the new buffer in
// Grade: DIFFER(498v496) -- Ego_d-faithful port (scoped_ptr new_index_buffer, per-block c_block / block_info / count / next_block_info, InitIndexBuffer); register tie: total_indices / new buffer in ebx/edi swapped v retail (edi/ebx), so the copy-loop flag reads go `mov bl ; test` instead of `cmp byte [..+9],0`
// Leaf: ?CreateDegenerateTriangleIndexBuffer@C3DPrimitive2@@AAE_NXZ
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_DATA
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
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
#include <string.h>
#include "engine/C3DPrimitive2.h"

bool C3DPrimitive2::CreateDegenerateTriangleIndexBuffer()
{
    unsigned long total_indices = IndexBuffer->GetSize() / sizeof(unsigned short);
    CArray<unsigned long> index_counts(StaticBlockCount);
    {
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            unsigned long count;
            if (c_block + 1 < StaticBlockCount) {
                N3DPrimitive::CStaticBlock& next_block_info = StaticBlocks[c_block + 1];
                count = next_block_info.StartIndex - block_info.StartIndex;
            } else
                count = total_indices - block_info.StartIndex;
            index_counts[c_block] = count;
        }
    }
    unsigned long degenerate_indices = 0;
    {
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            if (block_info.DegenerateTriangles)
                degenerate_indices += index_counts[c_block];
        }
    }
    boost::scoped_ptr<CIndexBuffer> new_index_buffer;
    if (degenerate_indices > 0) {
        new_index_buffer.reset(new CIndexBuffer);
        unsigned long new_byte_size = degenerate_indices * sizeof(unsigned short);
        InitIndexBuffer(*new_index_buffer, new_byte_size, InitFlags);
        const unsigned short* src_indices = (const unsigned short*)IndexBuffer->Lock(0, 0, total_indices * sizeof(unsigned short));
        unsigned short* dest_indices = (unsigned short*)new_index_buffer->Lock(0, 0, new_byte_size);
        unsigned long src_offset = 0;
        unsigned long dest_offset = 0;
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            unsigned long count = index_counts[c_block];
            if (block_info.DegenerateTriangles) {
                block_info.StartIndex = dest_offset;
                memcpy(dest_indices + dest_offset, src_indices + src_offset, count * sizeof(unsigned short));
                dest_offset += count;
            }
            src_offset += count;
        }
        new_index_buffer->Unlock();
        IndexBuffer->Unlock();
    }
    {
        for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block) {
            N3DPrimitive::CStaticBlock& block_info = StaticBlocks[c_block];
            if (!block_info.DegenerateTriangles) {
                block_info.StartIndex = 0;
                block_info.PrimitiveCount = 0;
            }
        }
    }
    IndexBuffer.swap(new_index_buffer);
    return true;
}
