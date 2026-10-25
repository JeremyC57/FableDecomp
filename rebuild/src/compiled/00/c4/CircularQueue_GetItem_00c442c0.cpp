// CircularQueue::GetItem(unsigned long index) -- the item stored in queue slot `index`
// Leaf: ?GetItem@CircularQueue@@QAEPAUCItem@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C442C0, 12 bytes (+ int3 padding), thiscall `ret 4`.
// Retail bytes:  mov eax,[ecx] ; mov ecx,[esp+4] ; mov eax,[eax+ecx*4] ; ret 4
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC442C0/0xC442D0 = CircularQueue::GetItem/GetTime"):
//   - the only caller is agent444's landed CAudioSource::QueueNextBuffer 0x00C72C40, which binds
//     ?GetItem@CircularQueue@@QAEPAUCItem@@K@Z / ?GetTime@CircularQueue@@QAE_JK@Z to these addresses and calls them on
//     the next slot after GetItemAtIndex 0x00C44430 (the landed CircularQueue row);
//   - the layout is the audio object's buffer queue (agent439 / the CAudioObject::ReleaseQueue rename 0x00C43810): items
//     +0, per-slot 64-bit times +4, capacity +8.
//   The CSmallVector_NavNode::index_b / CEditWorldMap::GetEditMap names are propagated; descriptive names (libpk: no
//   PDB/RTTI), agent444's signatures.
struct CItem;

class CircularQueue {
public:
    CItem**       m_items;                              // +0x00
    __int64*      m_times;                              // +0x04 per-slot times
    unsigned long m_capacity;                           // +0x08
    CItem* GetItem(unsigned long index);
};

CItem* CircularQueue::GetItem(unsigned long index)
{
    return m_items[index];
}
