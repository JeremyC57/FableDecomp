// CMemoryAllocatorFixedSize::Validate(SChunk* chunk) -- validate every area of one chunk (first at +0x14; the area header's own index at +0, the next index at +1, 0xFF ending the chain; step = (next - index) * the stride at +0xc2c): each payload (header + 4) is checked by the global allocator (0x013CAA90, 0x009F1F40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66260, length 64 bytes.
// Leaf: Validate
// Callees: 0x009F1F40 (thiscall on the global allocator @ 0x013CAA90, one argument). Shape: the area in esi, `this` in edi, the loop head aligned with a `nop`, `ret 4`.
// Naming (NF161, 2026-11-20, q4196): the global allocator 0x013CAA90 is the protected static ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A
//   (Ego_r.pdb / FableWin.pdb / Ego_d.pdb), reached through the public inline ?Get@CMemoryAllocator@@SIPAV1@XZ (FableWin.pdb / Ego_d.pdb) of
//   the shared engine/CMemoryAllocator.h; the empty check 0x009F1F40 is ?InternalValidateArea@CMemoryAllocator@@QAEXPAX@Z (Ego_r.pdb / FableWin.pdb, public; Ego_r donor order
//   GetNoAllocations / SetCurrentTrackingID / GetCurrentTrackingID / InternalValidateArea / InternalValidate = retail 0x009F1F10 / 1F20 / 1F30 /
//   1F40 / 1F50 by body). Was ?ValidateBlock@CMemoryAllocator_00a66260@@QAEXPAX@Z on ?g_pGlobalAllocator_013caa90@@3PAUCMemoryAllocator_00a66260@@A.
struct SFixedChunk_00a66260 {                      // a chunk of the fixed-size allocator: next at +8, the first area at +0x14
    unsigned char      _pad00[8];
    SFixedChunk_00a66260* m_pNext;                    // +0x08
    unsigned char      _pad0c[8];
    unsigned char*     m_pFirstArea;               // +0x14
};
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_INTERNALVALIDATEAREA
#include "rebuild_abi.h"
#include "engine/CMemoryAllocator.h"   // NF161 (q4196): was the stand-in CMemoryAllocator_00a66260 + extern g_pGlobalAllocator_013caa90

struct CMemoryAllocatorFixedSize {
    unsigned char _pad00[0xc2c];
    long          m_areaStride;                    // +0xc2c
    void Validate(SFixedChunk_00a66260* chunk);
};

void CMemoryAllocatorFixedSize::Validate(SFixedChunk_00a66260* chunk)
{
    unsigned char* area = chunk->m_pFirstArea;
    while (area) {
        CMemoryAllocator::Get()->InternalValidateArea(area + 4);
        unsigned char next = area[1];
        if (next == 0xff)
            break;
        area += (next - area[0]) * m_areaStride;
    }
}