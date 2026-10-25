// template<class T> PKCStaticQueue<T>::~PKCStaticQueue() -- non-virtual destructor of libpk's fixed-capacity queue: re-store the queue's vtable 0x012B2FC8 and free the record block at +0x08 (scalar operator delete)
// Leaf: ??1?$PKCStaticQueue@USFileRequest@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AEA0, 17 bytes, thiscall.
// Retail bytes:  mov eax,[ecx+8] ; push eax ; mov dword ptr [ecx],offset vftable ; call operator delete ; pop ecx ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 433 "0xC1AEA0 CLODMapOwner = PKCStaticQueue dtor"): the vtable it stores
//   is 0x012B2FC8, the PKCStaticQueue vtable the constructor 0x00C1B340 installs (slots Pop 0x00C1B370 -- whose trace
//   names "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" --, Push 0x00C1AEC0, GetCount 0x00C1AFB0, IsFull
//   0x00C1AFC0; no destructor slot), and +0x08 is the record block that constructor allocates with scalar operator new.
//   The PKCDefaultFileManager constructor 0x00C1AC40 names this as the unwind action of its +0x44 queue.
// Callee: scalar operator delete @ 0x00BFE9BC: the template's `delete[] m_pRecords` of a POD record calls the scalar `??3`
//   because this TU declares no `operator delete[]` (the CCodaFrame* instantiation 0x00C49AF0, whose TU does, calls 0x00BFEB1C).
// Label correction: the landed row was CLODMapOwner_012B2FC8::~CLODMapOwner_012B2FC8, and its `// Leaf: ??1CEngineLandscapeLODMap@`
//   (the manifest's propagated "~CEngineLandscapeLODMap") named no symbol of the object, so its grade fell back to the
//   largest function; descriptive names (no PDB covers libpk).
void __cdecl operator delete(void* block);                    // 0x00BFE9BC

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

template<class T> PKCStaticQueue<T>::~PKCStaticQueue()
{
    delete[] m_pRecords;
}

template PKCStaticQueue<SFileRequest>::~PKCStaticQueue();
