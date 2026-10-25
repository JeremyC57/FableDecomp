// template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity) -- constructs the fixed-capacity static queue (the file manager's request queue): stores the vtable and the capacity, allocates capacity seven-word records (scalar operator new) and zeroes the count and the head index
// Leaf: ??0?$PKCStaticQueue@USFileRequest@@@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B340, 45 bytes (+ int3 padding), __fastcall `ret 4`, returns `this`.
// Identity: libpk's fixed-capacity queue (0x14 bytes; vtable 0x012B2FC8 = Pop 0x00C1B370, whose empty-queue trace is
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" line 0x43 "Nothing in this queue - you should have checked
//   first!" -- hence the class name; a PKCStaticQueue<T> template: the same trace is in 4 Push bodies; no PDB name), Push 0x00C1AEC0 (checks the count against the capacity), GetCount 0x00C1AFB0,
//   IsFull 0x00C1AFC0; no destructor slot: the non-virtual destructor 0x00C1AEA0 re-stores the vtable and deletes
//   the record block). Callers: the file manager constructor 0x00C1AC98 (capacity 0x28) and the 0x00C1B6D0 queue
//   wrapper. The records are the seven-word requests the destructor 0x00C1A230 submits (0x1c bytes).
// Callee: scalar operator new @ 0x00BFEA1A: the template's `new T[capacity]` of a POD record calls the scalar `??2`
//   because this TU declares no `operator new[]` (the CCodaFrame* instantiation 0x00C49E00, whose TU does, calls 0x00BFEB22).
// Shape: initialiser list in declaration order; the vtable store is scheduled after the size push; one `xor eax,eax`
//   serves both zero stores.
// Label correction: the catalog's `CGameMap_AllocateTiles` is propagated (no CGameMap here); descriptive names.
// Naming (sweep1, 2026-10-25): the address suffix of the class (PKCStaticQueue_00c1b340) is dropped so the class has one
//   name across its rows (destructor 0x00C1AEA0, the PKCDefaultFileManager constructor 0x00C1AC40's +0x44 member).
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A

struct SFileRequest {                         // seven words: handle, key and five qualifiers
    void* m_hFile;
    void* m_pKey;
    long m_a;
    long m_b;
    long m_c;
    long m_d;
    long m_e;
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h); vtable 0x012B2FC8 for SFileRequest
public:
    PKCStaticQueue(long capacity);              // 0x00C1B340
    virtual void Pop(T* item);                  // slot 0 (0x00C1B370)
    virtual long Push(const T* item);           // slot 1 (0x00C1AEC0)
    virtual unsigned long GetCount();           // slot 2 (0x00C1AFB0)
    virtual long IsFull();                      // slot 3 (0x00C1AFC0)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C1AEA0 (non-virtual: no destructor slot)
};

template<class T> PKCStaticQueue<T>::PKCStaticQueue(long capacity)
    : m_capacity(capacity),
      m_pRecords(new T[capacity]),
      m_count(0),
      m_head(0)
{
}

template PKCStaticQueue<SFileRequest>::PKCStaticQueue(long);
