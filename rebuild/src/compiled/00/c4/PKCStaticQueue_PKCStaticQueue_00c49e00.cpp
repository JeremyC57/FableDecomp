// template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity) -- constructor of libpk's fixed-capacity queue, T = CCodaFrame* (vtable 0x012B80C8): store the capacity, allocate the frame-pointer slots with `new T[capacity]` (operator new[] 0x00BFEB22), zero the count and the head index
// Leaf: ??0?$PKCStaticQueue@PAVCCodaFrame@@@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C49E00, 45 bytes (+ int3 padding), __thiscall `ret 4`, returns `this`.
// Identity (sweep1, 2026-10-25): agent439 landed this as the non-template PKCFrameQueue::PKCFrameQueue. Its vtable 0x012B80C8 (Pop 0x00C49F10,
//   Push 0x00C49E30, GetCount 0x00C49B10, IsFull 0x00C49B20; destructor 0x00C49AF0) carries libpk's
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" asserts in Pop/Push ("Nothing in this queue - you should have checked
//   first!", "This queue is full!"), the same header as chunk 433's PKCStaticQueue (vtable 0x012B2FC8, ctor 0x00C1B340). Four
//   vtables (0x012B2FC8 / 0x012B8024 / 0x012B80C8 / 0x012B8364) over one header and one body shape are the instantiations of
//   one class template: PKCStaticQueue<CCodaFrame*> (descriptive; no PDB covers libpk). Owner: the CCodaFrameStreamer code 0x00C49860.
//   The manifest label is propagated.
// Callee: operator new[] 0x00BFEB22 (declared below; without it VC7.1 calls scalar new 0x00BFEA1A). Shape: the vtable store is
//   scheduled after the size push; one `xor eax,eax` serves both zero stores.
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22

class CCodaFrame;                               // the GameCODA streamed frame (.\CCodaFrameStreamer.cpp)

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    PKCStaticQueue(long capacity);              // 0x00C49E00
    virtual void Pop(T* item);                  // slot 0 (0x00C49F10)
    virtual long Push(const T* item);           // slot 1 (0x00C49E30)
    virtual unsigned long GetCount();           // slot 2 (0x00C49B10)
    virtual long IsFull();                      // slot 3 (0x00C49B20)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C49AF0 (non-virtual: no destructor slot)
};

template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity)
    : m_capacity(capacity),
      m_pRecords(new T[capacity]),
      m_count(0),
      m_head(0)
{
}

template PKCStaticQueue<CCodaFrame*>::PKCStaticQueue(long);
