// SBufferRecord00c9dce0::FreeBuffers() -- release every heap block a 0xB4-byte record owns (its sub-area, seven table buffers and three loose blocks) and zero the whole record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C9DCE0, length 87 bytes. Callees: the sub-area release @ 0x00C9C4C0 (on +0x10:
// frees that record's +8/+0x0c blocks and clears it; manifest "GFCheckTreeNodes"), MSVCR71 `free` through its import slot
// 0x01440154 (x10), the intrinsic memset.
// Label correction: the manifest calls this CRenderManager2D::SetupD3DUVBoxFromGraphic (propagated). It is the release
// routine of a plain heap-owning record in the C-style module around 0x00C8xxxx-0x00C9xxxx: its only caller (0x00C8F685, in
// "GraphicsResource_Cleanup" @ 0x00C8F650) calls it on the record pointer it holds and then frees the record itself. The
// record and member names are descriptive (no RTTI, no PDB match).
// Shape: the import address is hoisted into ebx for all ten calls (`__declspec(dllimport)` free), the seven-entry loop is
// strength-reduced to a pointer walk with a down-counter (`mov ebp,7 ... dec ebp ; jne`, its head 16-byte aligned), the three
// loose frees share one `add esp,0Ch`, and `memset(this, 0, sizeof *this)` is the inline `rep stosd` (0x2D dwords) through
// the `this` register.
// Leaf: FreeBuffers
#include <string.h>

extern "C" __declspec(dllimport) void __cdecl free(void* block);   // MSVCR71 import 0x01440154

struct SSubArea00c9dce0 {                               // 0x14 bytes, released by 0x00C9C4C0
    unsigned char _body[0x14];
};
void __fastcall ReleaseSubArea(SSubArea00c9dce0* area);   // 0x00C9C4C0

struct STableBuffer00c9dce0 {                           // 0x10 bytes
    void*         m_pData;                              // +0x00 (heap block)
    unsigned long m_size;                               // +0x04
    void*         m_pAux;                               // +0x08 (heap block in the last entry)
    unsigned long m_auxSize;                            // +0x0c
};

struct SBufferRecord00c9dce0 {
    unsigned char        _head[0x10];                   // +0x00
    SSubArea00c9dce0     m_area;                        // +0x10
    void*                m_pBlock24;                    // +0x24
    unsigned char        _pad28[0x08];                  // +0x28
    STableBuffer00c9dce0 m_tables[7];                   // +0x30 .. +0x9f
    void*                m_pBlockA0;                    // +0xa0
    unsigned char        _tailA4[0x10];                 // +0xa4 .. +0xb3
    void FreeBuffers();
};

void SBufferRecord00c9dce0::FreeBuffers()
{
    ReleaseSubArea(&m_area);
    for (int i = 0; i < 7; ++i)
        free(m_tables[i].m_pData);
    free(m_pBlock24);
    free(m_tables[6].m_pAux);
    free(m_pBlockA0);
    memset(this, 0, sizeof(*this));
}