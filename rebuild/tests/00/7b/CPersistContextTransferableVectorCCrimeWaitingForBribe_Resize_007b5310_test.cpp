// Behaviour test for 0x007B5310 CPersistContext_TransferableVector<CCrimeWaitingForBribe>::Resize: STLport
// resize(n) -> resize(n, T()): shrinking erases [start + n, finish), growing fill-inserts n - size zeroed records at
// finish. The two out-of-line vector members are simple working stand-ins over a fixed buffer.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif

class CCrimeWaitingForBribe { public:
    long DeedHandle;
    long Type;
    CCrimeWaitingForBribe() : DeedHandle(0), Type(0) { }
};

static int g_erases = 0, g_inserts = 0;
struct CCrimeWaitingForBribeVector {
    CCrimeWaitingForBribe* _M_start;
    CCrimeWaitingForBribe* _M_finish;
    CCrimeWaitingForBribe* _M_end_of_storage;
    CCrimeWaitingForBribe* begin() { return _M_start; }
    CCrimeWaitingForBribe* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCrimeWaitingForBribe* erase(CCrimeWaitingForBribe* first, CCrimeWaitingForBribe* last)
    {
        ++g_erases;
        CCrimeWaitingForBribe* out = first;
        for (CCrimeWaitingForBribe* p = last; p != _M_finish; ++p) *out++ = *p;
        _M_finish = out;
        return first;
    }
    void _M_fill_insert(CCrimeWaitingForBribe* position, unsigned long n, const CCrimeWaitingForBribe& x)
    {
        ++g_inserts;
        if (position != _M_finish) return;
        for (unsigned long i = 0; i < n; ++i) *_M_finish++ = x;
    }
    void insert(CCrimeWaitingForBribe* position, unsigned long n, const CCrimeWaitingForBribe& x) { _M_fill_insert(position, n, x); }
    __forceinline void resize(unsigned long newSize, const CCrimeWaitingForBribe& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    __forceinline void resize(unsigned long newSize) { resize(newSize, CCrimeWaitingForBribe()); }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(unsigned long count);
    CCrimeWaitingForBribeVector* TransVector;
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    TransVector->resize(count);
}
long CPersistContext_TransferableVector::GetCount() { return (long)TransVector->size(); }

int main()
{
    CCrimeWaitingForBribe storage[16];
    for (int i = 0; i < 16; ++i) { storage[i].DeedHandle = 100 + i; storage[i].Type = 7; }
    CCrimeWaitingForBribeVector vec; vec._M_start = storage; vec._M_finish = storage + 5; vec._M_end_of_storage = storage + 16;
    CPersistContext_TransferableVector wrapper; wrapper.TransVector = &vec;
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->Resize(3);                                   // shrink
    int ok = g_erases == 1 && g_inserts == 0 && hook->GetCount() == 3 && storage[2].DeedHandle == 102;
    hook->Resize(6);                                   // grow by three default records
    ok = ok && g_inserts == 1 && hook->GetCount() == 6 && storage[3].DeedHandle == 0 && storage[5].Type == 0 && storage[6].DeedHandle == 106;
    hook->Resize(6);                                   // same size: an empty fill-insert, as STLport
    ok = ok && g_inserts == 2 && g_erases == 1 && hook->GetCount() == 6;
    if (!ok) { std::printf("007b5310_TEST FAIL\n"); return 1; }
    std::printf("007b5310_TEST PASS\n");
    return 0;
}