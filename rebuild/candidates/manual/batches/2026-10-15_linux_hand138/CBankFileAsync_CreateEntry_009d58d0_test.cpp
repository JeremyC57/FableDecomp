// Behaviour test for 0x009d58d0 CBankFileAsync::CreateEntry
#include <cstdio>
static int g_deleted = 0;
struct CBankFileAsync;
struct CBankFileAsyncEntry {
    CBankFileAsync* m_pBank; unsigned long m_index; unsigned long m_alignedSize;
    inline CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index);
    virtual ~CBankFileAsyncEntry() { ++g_deleted; }
};
static void DeleteEntry(void* e) { delete static_cast<CBankFileAsyncEntry*>(e); }
struct SCountBlock {
    long m_references; void (*m_pfnDestroy)(void*); void* m_pObject;
    SCountBlock(void* o, void (*d)(void*)) : m_references(1), m_pfnDestroy(d), m_pObject(o) {}
};
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o, void (*d)(void*)) : m_pObject(o) { if (o) m_pCount = new SCountBlock(o, d); else m_pCount = 0; }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
struct CBankFileAsync {
    unsigned long GetEntryDataSize(unsigned long index) const { return index * 10 + 3; }
    unsigned long Align(unsigned long size) const { return (size + 15) & ~15UL; }
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long index)
    {
        CCountedPointer<CBankFileAsyncEntry> entry(new CBankFileAsyncEntry(this, index), DeleteEntry);
        return entry;
    }
};
inline CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index) : m_pBank(bank), m_index(index)
{
    m_alignedSize = m_pBank->Align(m_pBank->GetEntryDataSize(index));
}
int main()
{
    CBankFileAsync bank; bool ok;
    {
        CCountedPointer<CBankFileAsyncEntry> e = bank.CreateEntry(5);
        ok = e.m_pObject->m_pBank == &bank && e.m_pObject->m_index == 5 && e.m_pObject->m_alignedSize == 64 && e.m_pCount->m_references == 1;
    }
    ok = ok && g_deleted == 1;
    if (!ok) { std::printf("009d58d0_TEST FAIL\n"); return 1; }
    std::printf("009d58d0_TEST PASS\n");
    return 0;
}
