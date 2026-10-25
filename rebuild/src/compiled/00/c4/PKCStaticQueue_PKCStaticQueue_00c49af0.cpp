// template<class T> PKCStaticQueue<T>::~PKCStaticQueue() -- non-virtual destructor of libpk's fixed-capacity queue, T = CCodaFrame*: re-store the queue's vtable 0x012B80C8 and free the frame-pointer slots at +0x08 (`delete[]`, operator delete[] 0x00BFEB1C)
// Leaf: ??1?$PKCStaticQueue@PAVCCodaFrame@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49AF0, thiscall.
// Retail bytes:  mov eax,[ecx+8] ; push eax ; mov dword ptr [ecx],offset vftable ; call operator delete[] ; pop ecx ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439 "C49AF0 = the PKCFrameQueue dtor"): vtable 0x012B80C8 is the one
//   the landed constructor 0x00C49E00 installs; its Pop 0x00C49F10 and Push 0x00C49E30 carry libpk's
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" asserts ("Nothing in this queue - you should have checked first!",
//   "This queue is full!"), the same file as chunk 433's PKCStaticQueue (vtable 0x012B2FC8, ctor 0x00C1B340, dtor 0x00C1AEA0).
//   Four distinct vtables (0x012B2FC8 / 0x012B8024 / 0x012B80C8 / 0x012B8364) over one header file and one body shape are the
//   instantiations of one class template, so this is PKCStaticQueue<CCodaFrame*> (descriptive; no PDB covers libpk). Owner: the CCodaFrameStreamer (0x00C49860).
// Callee: operator delete[] 0x00BFEB1C (declared below; without the declaration VC7.1 calls the scalar ??3 0x00BFE9BC).
// Label correction: the landed row was CVtable_012b80c8::CVtable_012b80c8 (source class CLODMapOwner_012B80C8; its `// Leaf: ??1CEngineLandscapeLODMap@` named no symbol).
void __cdecl operator delete[](void* block);  // 0x00BFEB1C (MSVCR71 import thunk)

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

template<class T> PKCStaticQueue<T>::~PKCStaticQueue()
{
    delete[] m_pRecords;
}

template PKCStaticQueue<CCodaFrame*>::~PKCStaticQueue();
