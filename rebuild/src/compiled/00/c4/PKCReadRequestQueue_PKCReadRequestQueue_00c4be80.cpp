// PKCReadRequestQueue::PKCReadRequestQueue(long capacity) -- constructor of the buffered reader's fixed-capacity static queue (vtable 0x012B8364): store the capacity, allocate capacity one-word records with scalar operator new, zero the count and the head index
// Leaf: ??0PKCReadRequestQueue@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4BE80, 45 bytes (+ int3 padding), __fastcall `ret 4`, returns `this`.
// Library: libpk audio (no PDB/RTTI): descriptive names. Byte twin of chunk 433's PKCStaticQueue constructor
//   0x00C1B340 with 4-byte records (`shl eax,2`, scalar operator new 0x00BFEA1A, not new[]); vtable 0x012B8364: Pop
//   0x00C4BEB0, Push 0x00C4BB20, GetCount 0x00C4BC00, IsFull 0x00C4BC10 (landed destructor 0x00C4BB00). Only caller:
//   0x00C4B36F in the PKCBufferedReader code (.\PKCBufferedReader.cpp). The manifest's "PKCBufferedReader_SetBufferSize"
//   is a guess from the neighbouring strings.
// Shape: initialiser list in declaration order; the vtable store is scheduled after the size push; one `xor eax,eax`
//   serves both zero stores.
void* __cdecl operator new(unsigned int size);  // 0x00BFEA1A

struct SReadRequest {
    unsigned long m_id;
};

class PKCReadRequestQueue {
public:
    PKCReadRequestQueue(long capacity);
    virtual void Pop(SReadRequest* request);    // slot 0 (0x00C4BEB0)
    virtual long Push(const SReadRequest* request);   // slot 1 (0x00C4BB20)
    virtual long GetCount();                    // slot 2 (0x00C4BC00)
    virtual long IsFull();                      // slot 3 (0x00C4BC10)

    long          m_capacity;                   // +0x04
    SReadRequest* m_pRecords;                   // +0x08
    long          m_count;                      // +0x0c
    long          m_head;                       // +0x10
};

PKCReadRequestQueue::PKCReadRequestQueue(long capacity)
    : m_capacity(capacity),
      m_pRecords((SReadRequest*)operator new(capacity * sizeof(SReadRequest))),
      m_count(0),
      m_head(0)
{
}