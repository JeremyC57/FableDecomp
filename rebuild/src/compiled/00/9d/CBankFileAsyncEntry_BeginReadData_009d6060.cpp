// CBankFileAsyncEntry::BeginReadData(unsigned long start, void* buffer, unsigned long size, unsigned long bufferSize, EThreadedFilePriority priority) -- start an async read of `size` entry bytes from `start` into a caller-supplied buffer; returns the counted data object
// Leaf: ?BeginReadData@CBankFileAsyncEntry@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D6060, length 160 bytes, thiscall `ret 18h` (hidden
// CCountedPointer result + five arguments). PDB name (FableWin/Ego donors at 0x00B2A1B0 / 0x02FF7EC0):
// ?BeginReadData@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KPAXKKW4EThreadedFilePriority@@@Z.
// Catalogued as "CBankFileAsyncEntry_SetupReadRequest" (descriptive).
// Callees: operator new 0x00BFEA1A (the 0x48-byte data, the 12-byte count block), CBankFileAsyncData::CBankFileAsyncData
// (void* buffer, unsigned long size, CBankFileAsync& bank) 0x009D5D30 (three pushes in retail; the manifest's four-parameter
// signature is propagated from 0x009D5DF0), the count block's destroy function 0x009A42B0, CBankFileAsyncEntry::SetupRead
// 0x009D5970 (skip 0: a caller buffer needs no alignment slack), operator delete 0x00BFE9BC.
// Shape: the sibling BeginReadDataAutoAlloc 0x009D6100 (landed) without the alignment arithmetic: the counted pointer
// is a NAMED local built from the new object (inline count block: 1, deleter, object; a null object or block leaves
// both words 0) and returned by copy, so the copy's add-ref and the local's release survive as `inc ; dec` on the shared
// null test. Layout: CBankFileAsyncEntry (landed ctor 0x009D5260): m_pBank +4, m_index +8.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
void __fastcall Destroy_009a42b0(void* object);    // 0x009A42B0: the per-type deleter stored in the block

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_009a42b0), m_pObject(object) { }
};
template <class T> struct CCountedPointer {
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
class CBankFileAsync;
class CBankFileAsyncData {
public:
    unsigned char _body[0x48];
    CBankFileAsyncData(void* buffer, unsigned long size, CBankFileAsync& bank);   // 0x009D5D30
};

class CBankFileAsyncEntry {
public:
    virtual void v00();
    CBankFileAsync* m_pBank;                       // +0x04
    unsigned long   m_index;                       // +0x08
    unsigned long   m_alignedSize;                 // +0x0c
    void SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long readSize, unsigned long allocSize,
                   unsigned long skip, EThreadedFilePriority priority);   // 0x009D5970
    CCountedPointer<CBankFileAsyncData> BeginReadData(unsigned long start, void* buffer, unsigned long size,
                                                      unsigned long bufferSize, EThreadedFilePriority priority);
};

CCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadData(unsigned long start, void* buffer, unsigned long size,
                                                                       unsigned long bufferSize, EThreadedFilePriority priority)
{
    CCountedPointer<CBankFileAsyncData> data(new CBankFileAsyncData(buffer, size, *m_pBank));
    SetupRead(start, data.GetPtr(), size, bufferSize, 0, priority);
    return data;
}