// CEventDataBuffer::Allocate(long) -- allocates and zeroes room for `count` 12-byte event records, resetting the cursors (sets error -5 when out of memory)
// Leaf: Allocate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44360, 112 bytes; default /O2 TU, no frame, `ret 4`.
// Label correction: the catalog row "CTCHeroStats::AllocateEventData" is propagated; the body belongs to a library
// object of the 0xC3xxxx-0xC6xxxx block (only caller 0x00C43F1B). Class and member names are descriptive.
// Callees: the block's allocator @ 0x00C3C360 (__cdecl, forwards through a function pointer; landed as
//          "CTCHeroStats::InformOfEvent_a"), the error object @ 0x0143BB58's SetError(long) @ 0x00C3D090
//          (manifest "CDrawAutoPickups::ClearPrimitivesIfNecessary" is propagated).
// Shape: `size = count * 12` (`lea ebx,[ebp+ebp*2] ; shl ebx,2`) is computed once and reused for the memset
// (`rep stosd` + `rep stosb`); the data pointer is stored before its null test; the second pointer is
// `data + count` longs; the zero register from the null test feeds the three cursor stores.
struct CErrorState {
    void SetError(long code);                        // 0x00C3D090
};

extern CErrorState g_ErrorState;                     // 0x0143BB58

extern "C" void* __cdecl EventAlloc(unsigned long size);  // 0x00C3C360

extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct CEventDataBuffer {
    long* m_data;                                    // +0x00
    long* m_secondary;                               // +0x04 (data + count)
    long  m_capacity;                                // +0x08
    long  m_used;                                    // +0x0C
    long  m_read;                                    // +0x10
    long  m_write;                                   // +0x14

    bool Allocate(long count);
};

bool CEventDataBuffer::Allocate(long count)
{
    unsigned long size = count * 12;
    m_data = (long*)EventAlloc(size);
    if (m_data == 0) {
        g_ErrorState.SetError(-5);
        return false;
    }
    m_secondary = m_data + count;
    memset(m_data, 0, size);
    m_capacity = count;
    m_used = 0;
    m_read = 0;
    m_write = 0;
    return true;
}