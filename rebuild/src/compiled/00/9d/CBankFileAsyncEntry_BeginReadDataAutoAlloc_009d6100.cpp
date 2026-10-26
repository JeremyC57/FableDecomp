// CBankFileAsyncEntry::BeginReadDataAutoAlloc(unsigned long start, unsigned long size, EThreadedFilePriority priority) -- start an async read of `size` entry bytes from `start` into pool memory: non-cached banks round the start down to the alignment (the skip is remembered), the read and allocation sizes are aligned; returns the counted data object
// Leaf: BeginReadDataAutoAlloc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D6100, length 235 bytes (+5 int3), ret 0x10 (hidden CCountedPointer result).
// Callees: CBankFile::GetAlignment() @ 0x009CBB80 (catalog "CWorld::DrawGetEnvironment", propagated), CBankFile::Align
// @ 0x009CBB90, operator new @ 0x00BFEA1A (the 0x48-byte data, the 12-byte count block), CBankFileAsyncData::CBankFileAsyncData
// (unsigned long, unsigned long, CMovableResourceMemoryPool*, CBankFileAsync&) @ 0x009D5DF0, the count block's destroy
// function @ 0x009A42B0, CBankFileAsyncEntry::SetupRead @ 0x009D5970, operator delete @ 0x00BFE9BC.
// Shape: the read size (skip + size) and the aligned size are homed in the dead size/start argument slots; the counted
// pointer is a NAMED local built from the new object (inline count block: 1, deleter, object) and returned by copy, so the
// copy's add-ref and the local's release survive as `inc ; dec` on the shared null test.
// Layout: CBankFileAsyncEntry (landed ctor 0x009D5260): m_pBank +4, m_index +8; retail CBankFileAsync: LoadingMemoryPool
// +0x118, NonCached +0x148.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
void __fastcall Destroy_009a42b0(void* object);    // 0x009A42B0: the per-type deleter stored in the block

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_009a42b0), m_pObject(object) { }
};
template <class T> class CCountedPointer { public:
    T* Data;                                       // +0
    SCountBlock* Info;                             // +4
    T* GetPtr() const { return Data; }
    CCountedPointer(T* object) : Data(object), Info(object ? new SCountBlock(object) : 0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
    }
};
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };
class CMovableResourceMemoryPool;
class CBankFileAsync;
class CBankFileAsyncData {
public:
    unsigned char _body[0x48];
    CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* pool, CBankFileAsync& bank);   // 0x009D5DF0
};
class CBankFileAsync {
public:
    unsigned char _pad[0x118];
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;   // +0x118
    unsigned char _pad120[0x28];                   // +0x120 .. +0x147
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    unsigned long GetAlignment() const;            // 0x009CBB80 (CBankFile)
    unsigned long Align(unsigned long size) const; // 0x009CBB90 (CBankFile)
};

class CBankFileAsyncEntry {
public:
    virtual void v00();
    CBankFileAsync* m_pBank;                       // +0x04
    unsigned long   m_index;                       // +0x08
    unsigned long   m_alignedSize;                 // +0x0c
    void SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long readSize, unsigned long allocSize,
                   unsigned long skip, EThreadedFilePriority priority);   // 0x009D5970
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long start, unsigned long size, EThreadedFilePriority priority);
};

CCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadDataAutoAlloc(unsigned long start, unsigned long size,
                                                                                EThreadedFilePriority priority)
{
    unsigned long alignedStart;
    unsigned long skip;
    if (m_pBank->NonCached) {
        unsigned long alignment = m_pBank->GetAlignment();
        alignedStart = start / alignment * alignment;
        skip = start - alignedStart;
    } else {
        alignedStart = start;
        skip = 0;
    }
    unsigned long readSize = skip + size;
    unsigned long allocSize = m_pBank->Align(readSize);
    CCountedPointer<CBankFileAsyncData> data(new CBankFileAsyncData(size, skip, m_pBank->LoadingMemoryPool.GetPtr(), *m_pBank));
    SetupRead(alignedStart, data.GetPtr(), readSize, allocSize, skip, priority);
    return data;
}