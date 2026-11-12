// C3DPrimitive2::CreateDegenerateTriangleIndexBuffer() -- copy the index runs of every degenerate-triangle static block into a new index buffer (rebasing those blocks), clear the other blocks and replace the primitive's index buffer
// Grade: DIFFER(496v496) -- register colouring in the copy loop: the loop counter i (c2 prio 84) loses to dest_index (94), so ours keeps dest_index in eax and spills i in esi where retail keeps i in eax and dest_index in memory through edi; the flag is read with `mov bl` (retail `cmp byte`) and the loop-2 alignment jmp is missing. `/ sizeof(unsigned short)` already gives retail's unsigned shr.
// Leaf: ?CreateDegenerateTriangleIndexBuffer@C3DPrimitive2@@AAE_NXZ
// Retail: Fable.exe 0x00A90260, 496 bytes, thiscall `ret` (bool true in al; Ego_r.pdb name above).
// Callees: CIndexBuffer::GetSize 0x00AC1F10, CArray<unsigned long>(unsigned int) 0x00A96100 (out of line), operator new 0x00BFEA1A,
//   CIndexBuffer() 0x00AC1EC0, CIndexBuffer::Initialise 0x00AC1F70, Lock 0x00AC1FB0, Unlock 0x00AC1F00, the old buffer's virtual
//   deleting destructor (slot 0, through boost::scoped_ptr::reset), free 0x00BFEA14 (the local array's storage).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#include <string.h>
#include "engine/CArray.h"
#include "engine/C3DPrimitive2.h"

bool C3DPrimitive2::CreateDegenerateTriangleIndexBuffer()
{
    unsigned long total_indices = IndexBuffer->GetSize() / sizeof(unsigned short);
    CArray<unsigned long> block_sizes(StaticBlockCount);
    unsigned long i;
    for (i = 0; i < StaticBlockCount; ++i) {
        N3DPrimitive::CStaticBlock& block = StaticBlocks[i];
        block_sizes[i] = (i + 1 < StaticBlockCount ? (&block)[1].StartIndex : total_indices) - block.StartIndex;
    }
    unsigned long degenerate_indices = 0;
    for (i = 0; i < StaticBlockCount; ++i)
        if (StaticBlocks[i].DegenerateTriangles)
            degenerate_indices += block_sizes[i];
    CIndexBuffer* new_buffer = 0;
    if (degenerate_indices > 0) {
        CIndexBuffer* buffer = new CIndexBuffer();
        new_buffer = buffer;
        unsigned long usage = 8;
        long pool = 1;
        if (InitFlags & 1) {
            usage = 0x18;
            pool = 2;
        }
        buffer->Initialise(degenerate_indices * 2, usage, (EIndexBufferFormat)101, (EIndexBufferPool)pool, (EPrimitiveType)4);
        unsigned short* source = (unsigned short*)IndexBuffer->Lock(0, 0, total_indices * 2);
        unsigned short* dest = (unsigned short*)buffer->Lock(0, 0, degenerate_indices * 2);
        unsigned long source_index = 0;
        unsigned long dest_index = 0;
        for (i = 0; i < StaticBlockCount; ++i) {
            N3DPrimitive::CStaticBlock& block = StaticBlocks[i];
            unsigned long size = block_sizes[i];
            if (block.DegenerateTriangles) {
                block.StartIndex = dest_index;
                memcpy(dest + dest_index, source + source_index, size * 2);
                dest_index += size;
            }
            source_index += size;
        }
        buffer->Unlock();
        IndexBuffer->Unlock();
    }
    for (i = 0; i < StaticBlockCount; ++i) {
        N3DPrimitive::CStaticBlock& block = StaticBlocks[i];
        if (!block.DegenerateTriangles) {
            block.StartIndex = 0;
            block.PrimitiveCount = 0;
        }
    }
    IndexBuffer.reset(new_buffer);
    return true;
}
