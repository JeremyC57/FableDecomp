// CircularQueue::Allocate(long) -- allocates and zeroes one block for `count` queue slots (a 4-byte item pointer and an 8-byte time each), resetting the cursors (sets error -5 when out of memory)
// Leaf: ?Allocate@CircularQueue@@QAE_NJ@Z
// Vendor naming wave V1 (one address, one name): 0x00C3C360 _CodaMalloc (was _EventAlloc)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44360, 112 bytes; default /O2 TU, no frame, `ret 4`.
// Label correction: the catalog row "CTCHeroStats::AllocateEventData" is propagated; the body belongs to a library
// object of the 0xC3xxxx-0xC6xxxx block (only caller 0x00C43F1B). Class and member names are descriptive.
// Callees: the block's allocator @ 0x00C3C360 (__cdecl, forwards through a function pointer; landed as
//          "CTCHeroStats::InformOfEvent_a"), the error object @ 0x0143BB58's SetError(long) @ 0x00C3D090
//          (manifest "CDrawAutoPickups::ClearPrimitivesIfNecessary" is propagated).
// Shape: `size = count * 12` (`lea ebx,[ebp+ebp*2] ; shl ebx,2`) is computed once and reused for the memset
// (`rep stosd` + `rep stosb`); the data pointer is stored before its null test; the second pointer is
// `items + count` pointers (the times); the zero register from the null test feeds the three cursor stores.
class CCodaContext { public:
    void SetError(int a0);   // 0x00C3D090 (Ego_r.pdb ?SetError@CCodaContext@@QAEXH@Z)
};
struct CErrorState {
};

extern CErrorState g_ErrorState;                     // 0x0143BB58

extern "C" void* __cdecl CodaMalloc(unsigned long size);  // 0x00C3C360

extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

class CSoundBuffer;

// The audio object's +0x54 buffer queue: the landed sibling model of 0x00C437C0 / 0x00C43810 / 0x00C44550 (descriptive, libpk
// has no PDB). Was CEventDataBuffer (m_data / m_secondary / m_capacity / m_used / m_read / m_write); the only caller 0x00C43EC0
// allocates the queue with it and frees it on error with CircularQueue::Release 0x00C44550, which frees this one block.
class CircularQueue {
public:
    CSoundBuffer**    m_items;                      // +0x00 (the block)
    unsigned __int64* m_times;                      // +0x04 (items + count)
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18

    bool Allocate(long count);
};

bool CircularQueue::Allocate(long count)
{
    unsigned long size = count * 12;
    m_items = (CSoundBuffer**)CodaMalloc(size);
    if (m_items == 0) {
        ((CCodaContext*)&g_ErrorState)->SetError(-5);
        return false;
    }
    m_times = (unsigned __int64*)(m_items + count);
    memset(m_items, 0, size);
    m_capacity = count;
    m_head = 0;
    m_tail = 0;
    m_lastItem = 0;
    return true;
}
