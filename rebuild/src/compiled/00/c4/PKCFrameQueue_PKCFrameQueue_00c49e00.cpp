// PKCFrameQueue::PKCFrameQueue(long capacity) -- constructor of the frame streamer's fixed-capacity static queue (vtable 0x012B80C8): store the capacity, allocate the pointer slots with `new[]`, zero the count and the head index
// Leaf: ??0PKCFrameQueue@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49E00, 45 bytes (+ int3 padding), __fastcall `ret 4`, returns `this`.
// Library: libpk audio (no PDB/RTTI): descriptive names. Same shape as chunk 433's PKCStaticQueue constructor
//   0x00C1B340 (libpk PKCStaticQueue.h: Pop/Push/GetCount/IsFull in slots 0..3), here over 4-byte slots allocated
//   with operator new[] 0x00BFEB22; vtable 0x012B80C8: Pop 0x00C49F10, Push 0x00C49E30, GetCount 0x00C49B10, IsFull
//   0x00C49B20 (landed destructor 0x00C49AF0). Only caller: 0x00C49860 in the CCodaFrameStreamer code (.\CCodaFrameStreamer.cpp).
//   The manifest's "LocalDetailPrimitive_AllocateArray" is propagated.
// Shape: initialiser list in declaration order; the vtable store is scheduled after the size push; one `xor eax,eax`
//   serves both zero stores.
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22 (without it VC7.1 calls scalar new 0x00BFEA1A)

struct CCodaFrame;

class PKCFrameQueue {
public:
    PKCFrameQueue(long capacity);
    virtual void Pop(CCodaFrame** frame);       // slot 0 (0x00C49F10)
    virtual long Push(CCodaFrame* const* frame);   // slot 1 (0x00C49E30)
    virtual long GetCount();                    // slot 2 (0x00C49B10)
    virtual long IsFull();                      // slot 3 (0x00C49B20)

    long         m_capacity;                    // +0x04
    CCodaFrame** m_pSlots;                      // +0x08
    long         m_count;                       // +0x0c
    long         m_head;                        // +0x10
};

PKCFrameQueue::PKCFrameQueue(long capacity)
    : m_capacity(capacity),
      m_pSlots(new CCodaFrame*[capacity]),
      m_count(0),
      m_head(0)
{
}