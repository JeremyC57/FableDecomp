// Behaviour test for 009b31a0 (ReleaseCountedPointerAt0C): the record's counted pointer at +0xc drops one reference (the last one notifies the pointee) and is nulled; a null pointer is left alone.
#include <cstdio>
#include <new>
static int g_lastReferences = 0;
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase() { }
    virtual void OnLastReference() { ++g_lastReferences; }
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(0) { }
    void Release()
    {
        if (--IVRefCount == 0)
            OnLastReference();
    }
};
template <class T> struct CIVCountedPointer {
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    ~CIVCountedPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};
struct SCountedRecord10 {
    long m_words[3];
    CIVCountedPointer<CIVCountedPointeeBase> m_pObject;
};
void ReleaseCountedPointerAt0C(SCountedRecord10* record)
{
    record->~SCountedRecord10();
}
int main()
{
    CIVCountedPointeeBase shared;
    shared.IVRefCount = 2;
    union { void* p; char bytes[sizeof(SCountedRecord10) * 3]; } storage;
    SCountedRecord10* records = reinterpret_cast<SCountedRecord10*>(storage.bytes);
    for (int i = 0; i < 3; ++i)
        new (&records[i]) SCountedRecord10();
    records[0].m_pObject.m_p = &shared;
    records[1].m_pObject.m_p = &shared;
    ReleaseCountedPointerAt0C(&records[0]);
    if (shared.IVRefCount != 1 || g_lastReferences != 0 || records[0].m_pObject.m_p != 0) { std::printf("009b31a0_TEST FAIL\n"); return 1; }
    ReleaseCountedPointerAt0C(&records[1]);
    if (shared.IVRefCount != 0 || g_lastReferences != 1 || records[1].m_pObject.m_p != 0) { std::printf("009b31a0_TEST FAIL\n"); return 1; }
    ReleaseCountedPointerAt0C(&records[2]);
    if (g_lastReferences != 1) { std::printf("009b31a0_TEST FAIL\n"); return 1; }
    std::printf("009b31a0_TEST PASS\n");
    return 0;
}