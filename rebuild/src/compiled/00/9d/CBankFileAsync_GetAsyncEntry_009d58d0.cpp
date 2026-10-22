// CBankFileAsync::GetAsyncEntry(unsigned long index) -- allocate a CBankFileAsyncEntry bound to this bank and entry index and return it in a fresh counted pointer
// Leaf: CreateEntry
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D58D0, 152 bytes (+ int3 padding).
// Label correction: the manifest's "CEngineWorldMap::CreateNewBankMap" is propagated over byte-similar factories; the
//   object built here stores the CBankFileAsyncEntry vtable 0x0129B880 (rtti-vtables.tsv) and its constructor body is the
//   landed 0x009D5260 inlined, so `this` is the CBankFileAsync. The real method name is unknown (descriptive name).
// Callees: operator new @ 0x00BFEA1A (0x10, then the 12-byte count block), CBase::CBase @ 0x0099A2F0,
//   CBankFile::GetEntryDataSize @ 0x009CC240, CBankFile::Align @ 0x009CBB90, operator delete @ 0x00BFE9BC, and the
//   entry deleter @ 0x009D6A60 (`if (p) p->vdtor(1)`: a virtual `delete p`) stored in the count block.
// Shape: `__fastcall` with the hidden return slot first on the stack (`ret 8`). `CCountedPointer<T> p(new T(...));
//   return p;` -- no NRVO, so the named local is copied into the hidden slot (add-reference) and destroyed (release):
//   the merged `inc ecx ; mov eax,ecx ; dec eax` with both count stores kept. The inlined entry constructor calls
//   GetEntryDataSize on the bank still in edi and re-reads the stored member for Align (as in 0x009D5260).
struct CBase {                       // the 4-slot root at 0x0099A2F0, stores vtable 0x0129A7C4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    CBase();                         // 0x0099A2F0
};

struct CBankFileAsync;

struct CBankFileAsyncEntry : CBase {
    CBankFileAsync* m_pBank;         // +0x04
    unsigned long   m_index;         // +0x08
    unsigned long   m_alignedSize;   // +0x0c
    inline CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index);
};

void __fastcall DeleteBankFileAsyncEntry(void* entry);   // 0x009D6A60

struct SCountBlock {
    long  m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);      // +4: per-type deleter, object in ecx
    void* m_pObject;                             // +8
    SCountBlock(void* object, void (__fastcall* destroy)(void*)) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template <class T>
struct CCountedPointer {
    T*           m_pObject;                      // +0
    SCountBlock* m_pCount;                       // +4
    CCountedPointer(T* object, void (__fastcall* destroy)(void*)) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(object, destroy);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};

struct CBankFileAsync {
    unsigned long GetEntryDataSize(unsigned long index) const;   // 0x009CC240 (CBankFile)
    unsigned long Align(unsigned long size) const;               // 0x009CBB90 (CBankFile)
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);
};

inline CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index)
    : CBase(), m_pBank(bank), m_index(index)
{
    m_alignedSize = m_pBank->Align(m_pBank->GetEntryDataSize(index));
}

CCountedPointer<CBankFileAsyncEntry> CBankFileAsync::GetAsyncEntry(unsigned long index)
{
    CCountedPointer<CBankFileAsyncEntry> entry(new CBankFileAsyncEntry(this, index), DeleteBankFileAsyncEntry);
    return entry;
}