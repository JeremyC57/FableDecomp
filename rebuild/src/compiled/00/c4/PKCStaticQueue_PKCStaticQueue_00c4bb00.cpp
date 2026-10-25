// template<class T> PKCStaticQueue<T>::~PKCStaticQueue() -- non-virtual destructor of libpk's fixed-capacity queue, T = SReadRequest: re-store the queue's vtable 0x012B8364 and free the one-word read-request records at +0x08 (scalar operator delete 0x00BFE9BC)
// Leaf: ??1?$PKCStaticQueue@USReadRequest@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4BB00, thiscall.
// Retail bytes:  mov eax,[ecx+8] ; push eax ; mov dword ptr [ecx],offset vftable ; call operator delete ; pop ecx ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439 "C4BB00 = the PKCReadRequestQueue dtor"): vtable 0x012B8364 is the one
//   the landed constructor 0x00C4BE80 installs; its Pop 0x00C4BEB0 and Push 0x00C4BB20 carry libpk's
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" asserts ("Nothing in this queue - you should have checked first!",
//   "This queue is full!"), the same file as chunk 433's PKCStaticQueue (vtable 0x012B2FC8, ctor 0x00C1B340, dtor 0x00C1AEA0).
//   Four distinct vtables (0x012B2FC8 / 0x012B8024 / 0x012B80C8 / 0x012B8364) over one header file and one body shape are the
//   instantiations of one class template, so this is PKCStaticQueue<SReadRequest> (descriptive; no PDB covers libpk). Owner: the PKCBufferedReader (0x00C4B36F).
// Callee: scalar operator delete 0x00BFE9BC: the template's `delete[]` of a POD record calls the scalar ??3 because this TU, like
//   retail's PKCBufferedReader TU, declares no operator delete[] (its constructor 0x00C4BE80 likewise calls scalar new 0x00BFEA1A).
// Label correction: the landed row was CVtable_012b8364::CVtable_012b8364 (source class CLODMapOwner_012B8364; its `// Leaf: ??1CEngineLandscapeLODMap@` named no symbol).
void __cdecl operator delete(void* block);    // 0x00BFE9BC

struct SReadRequest {                           // one word: the buffered reader's request id (agent439's 0x00C4BE80 model)
    unsigned long m_id;
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    PKCStaticQueue(long capacity);              // 0x00C4BE80
    virtual void Pop(T* item);                  // slot 0 (0x00C4BEB0)
    virtual long Push(const T* item);           // slot 1 (0x00C4BB20)
    virtual unsigned long GetCount();           // slot 2 (0x00C4BC00)
    virtual long IsFull();                      // slot 3 (0x00C4BC10)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C4BB00 (non-virtual: no destructor slot)
};

template<class T> PKCStaticQueue<T>::~PKCStaticQueue()
{
    delete[] m_pRecords;
}

template PKCStaticQueue<SReadRequest>::~PKCStaticQueue();
