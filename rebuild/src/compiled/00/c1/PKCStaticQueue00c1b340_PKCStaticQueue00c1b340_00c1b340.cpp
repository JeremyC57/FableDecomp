// PKCStaticQueue_00c1b340::PKCStaticQueue_00c1b340(long capacity) -- constructs the fixed-capacity static queue (the file manager's request queue): stores the vtable and the capacity, allocates capacity seven-word records (scalar operator new) and zeroes the count and the head index
// Leaf: ??0PKCStaticQueue_00c1b340@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B340, 45 bytes (+ int3 padding), __fastcall `ret 4`, returns `this`.
// Identity: libpk's fixed-capacity queue (0x14 bytes; vtable 0x012B2FC8 = Pop 0x00C1B370, whose empty-queue trace is
//   "d:\dev\audio\ver_fable_1.5_b\libpk\PKCStaticQueue.h" line 0x43 "Nothing in this queue - you should have checked
//   first!" -- hence the class name; it may be a PKCStaticQueue<T> template instance, no PDB name), Push 0x00C1AEC0 (checks the count against the capacity), GetCount 0x00C1AFB0,
//   IsFull 0x00C1AFC0; no destructor slot: the non-virtual destructor 0x00C1AEA0 re-stores the vtable and deletes
//   the record block). Callers: the file manager constructor 0x00C1AC98 (capacity 0x28) and the 0x00C1B6D0 queue
//   wrapper. The records are the seven-word requests the destructor 0x00C1A230 submits (0x1c bytes).
// Callee: scalar operator new @ 0x00BFEA1A (`operator new(n * 0x1c)`, not new[]).
// Shape: initialiser list in declaration order; the vtable store is scheduled after the size push; one `xor eax,eax`
//   serves both zero stores.
// Label correction: the catalog's `CGameMap_AllocateTiles` is propagated (no CGameMap here); descriptive names.
void* __cdecl operator new(unsigned int size);                 // 0x00BFEA1A

struct SRequest_00c1b340 {                         // seven words: handle, key and five qualifiers
    void* m_hFile;
    void* m_pKey;
    long m_a;
    long m_b;
    long m_c;
    long m_d;
    long m_e;
};

class PKCStaticQueue_00c1b340 {
public:
    PKCStaticQueue_00c1b340(long capacity);
    virtual void Pop(SRequest_00c1b340* request);                   // slot 0 (0x00C1B370)
    virtual long Push(const SRequest_00c1b340* request);            // slot 1 (0x00C1AEC0)
    virtual long GetCount();                                   // slot 2 (0x00C1AFB0)
    virtual long IsFull();                                     // slot 3 (0x00C1AFC0)

    long             m_capacity;                               // +0x04
    SRequest_00c1b340*    m_pRecords;                               // +0x08
    long             m_count;                                  // +0x0c
    long             m_head;                                   // +0x10
};

PKCStaticQueue_00c1b340::PKCStaticQueue_00c1b340(long capacity)
    : m_capacity(capacity),
      m_pRecords((SRequest_00c1b340*)operator new(capacity * sizeof(SRequest_00c1b340))),
      m_count(0),
      m_head(0)
{
}