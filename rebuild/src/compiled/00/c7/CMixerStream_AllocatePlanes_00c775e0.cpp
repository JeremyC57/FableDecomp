// CMixerStream::AllocatePlanes() -- (re)allocate the stream's three equal byte planes as one zeroed block of 3 * planeSize bytes; false when the allocation fails
// Leaf: ?AllocatePlanes@CMixerStream@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C775E0, length 109 bytes (+ int3 padding), plain `ret`, bool in al.
// Callees: the libpk allocator wrappers MixerAlloc 0x00C3C360 / MixerFree 0x00C3C370 (__cdecl, `add esp,4`; both
// forward to hooks at 0x0139C97C / 0x0139C980). Caller: the software mixer's stream setup at 0x00C6A96F (`mov
// ecx,esi`), which tests al and bails out on false.
// Library: libpk software mixer (no PDB/RTTI); class and member names are descriptive. The manifest's
// "CTCHeroStats_NotifyObservers" is a guess with no relation to the body.
// Shape: the old block is freed and all three plane pointers cleared before the new allocation; the result is
// stored before its null test (`mov [esi+4],edi` between `test` and `jne`); the size is re-read once after the call
// for both plane offsets and the memset intrinsic (`rep stosd` + `rep stosb` on edi = the block).
#include <string.h>

void* __cdecl MixerAlloc(unsigned long size);      // 0x00C3C360
void __cdecl MixerFree(void* block);               // 0x00C3C370

struct CMixerStream {
    unsigned char _unk00[4];
    unsigned char* m_pPlaneA;                      // +0x04 owns the block
    unsigned char* m_pPlaneB;                      // +0x08 = A + planeSize
    unsigned char* m_pPlaneC;                      // +0x0c = A + 2 * planeSize
    unsigned long m_planeSize;                     // +0x10 bytes per plane
    bool AllocatePlanes();
};

bool CMixerStream::AllocatePlanes()
{
    if (m_pPlaneA != 0) {
        MixerFree(m_pPlaneA);
        m_pPlaneA = 0;
        m_pPlaneB = 0;
        m_pPlaneC = 0;
    }
    m_pPlaneA = (unsigned char*)MixerAlloc(m_planeSize * 3);
    if (m_pPlaneA == 0)
        return false;
    m_pPlaneB = m_pPlaneA + m_planeSize;
    m_pPlaneC = m_pPlaneA + m_planeSize * 2;
    memset(m_pPlaneA, 0, m_planeSize * 3);
    return true;
}