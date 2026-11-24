// CircularQueue::Release() -- when the queue has a capacity: clear it (release the queued buffers), free its item / time block through the pool free, and zero the item, time and capacity words
// Leaf: ?Release@CircularQueue@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44550, 48 bytes, thiscall, `ret`.
// Identity: libpk's audio-object buffer queue (+0x54 of CAudioObject, vtable 0x012B5A88), the landed sibling model of
//   0x00C437C0 / 0x00C43810 (items +0, 64-bit times +4, capacity +8, head +0xc, tail +0x10, last item +0x14, stats +0x18).
//   Name descriptive (no PDB covers libpk), matching the siblings CircularQueue::GetInfo 0x00C442F0 / Clear 0x00C443D0.
//   Callers: CAudioObject::ReleaseQueue 0x00C43810 and the allocation error path of 0x00C43EC0 call it; the inline
//   ~CircularQueue expands to it in the owner's destructor 0x00C3CF40, and its out-of-line copy 0x00C3CF30 is `jmp Release`
//   (the target of 0x00C3CF40's unwind funclet 0x01214F88).
// Callees: CircularQueue::Clear 0x00C443D0, the `__cdecl` block free 0x00C3C370 (one block holds the items and times;
//   CircularQueue::Allocate 0x00C44360 allocates it).
// Shape: the three zero stores are immediates after the free's `add esp,4`.
// Label correction: was the July placeholder CMarkedBuffer_00c44550::Release (m_pBlock / m_pCursor / m_pBuffer) calling ClearMarks.
void __cdecl FreeBlock_00c3c370(void* block);      // 0x00C3C370

class CSoundBuffer;

class CircularQueue {                               // the audio object's buffer queue (+0x54)
public:
    void Clear();                                   // 0x00C443D0
    void Release();
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
};

void CircularQueue::Release()
{
    if (m_capacity) {
        Clear();
        FreeBlock_00c3c370(m_items);
        m_items = 0;
        m_times = 0;
        m_capacity = 0;
    }
}
