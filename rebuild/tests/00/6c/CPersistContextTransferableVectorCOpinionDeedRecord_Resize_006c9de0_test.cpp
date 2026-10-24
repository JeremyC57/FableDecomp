// Behaviour test for 0x006C9DE0 CPersistContext_TransferableVector<COpinionDeedRecord>::Resize: STLport
// resize(n) -> resize(n, T()): shrinking erases [start + n, finish), growing fill-inserts n - size zeroed records at
// finish. The two out-of-line vector members are simple working stand-ins over a fixed buffer.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif

struct COpinionDeedRecord {
    long m_deedType;
    long m_value;
    COpinionDeedRecord() : m_deedType(0), m_value(0) { }
};

static int g_erases = 0, g_inserts = 0;
struct COpinionDeedRecordVector {
    COpinionDeedRecord* _M_start;
    COpinionDeedRecord* _M_finish;
    COpinionDeedRecord* _M_end_of_storage;
    COpinionDeedRecord* begin() { return _M_start; }
    COpinionDeedRecord* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    COpinionDeedRecord* erase(COpinionDeedRecord* first, COpinionDeedRecord* last)
    {
        ++g_erases;
        COpinionDeedRecord* out = first;
        for (COpinionDeedRecord* p = last; p != _M_finish; ++p) *out++ = *p;
        _M_finish = out;
        return first;
    }
    void _M_fill_insert(COpinionDeedRecord* position, unsigned long n, const COpinionDeedRecord& x)
    {
        ++g_inserts;
        if (position != _M_finish) return;
        for (unsigned long i = 0; i < n; ++i) *_M_finish++ = x;
    }
    void insert(COpinionDeedRecord* position, unsigned long n, const COpinionDeedRecord& x) { _M_fill_insert(position, n, x); }
    __forceinline void resize(unsigned long newSize, const COpinionDeedRecord& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    __forceinline void resize(unsigned long newSize) { resize(newSize, COpinionDeedRecord()); }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(unsigned long count);
    COpinionDeedRecordVector* TransVector;
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    TransVector->resize(count);
}
long CPersistContext_TransferableVector::GetCount() { return (long)TransVector->size(); }

int main()
{
    COpinionDeedRecord storage[16];
    for (int i = 0; i < 16; ++i) { storage[i].m_deedType = 100 + i; storage[i].m_value = 7; }
    COpinionDeedRecordVector vec; vec._M_start = storage; vec._M_finish = storage + 5; vec._M_end_of_storage = storage + 16;
    CPersistContext_TransferableVector wrapper; wrapper.TransVector = &vec;
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->Resize(3);                                   // shrink
    int ok = g_erases == 1 && g_inserts == 0 && hook->GetCount() == 3 && storage[2].m_deedType == 102;
    hook->Resize(6);                                   // grow by three default records
    ok = ok && g_inserts == 1 && hook->GetCount() == 6 && storage[3].m_deedType == 0 && storage[5].m_value == 0 && storage[6].m_deedType == 106;
    hook->Resize(6);                                   // same size: an empty fill-insert, as STLport
    ok = ok && g_inserts == 2 && g_erases == 1 && hook->GetCount() == 6;
    if (!ok) { std::printf("006c9de0_TEST FAIL\n"); return 1; }
    std::printf("006c9de0_TEST PASS\n");
    return 0;
}