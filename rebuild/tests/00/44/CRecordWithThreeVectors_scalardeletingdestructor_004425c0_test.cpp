#include <cstdio>
#include <stdlib.h>
template <class T> class CPodVector {                   // STLport vector of plain elements
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    ~CPodVector() { if (_M_start) free(_M_start); }
};
class CRecordWithThreeVectors {
public:
    long              Key;                               // +0x00
    CPodVector<long>  First;                             // +0x04
    CPodVector<long>  Second;                            // +0x10
    CPodVector<long>  Third;                             // +0x1c
};
void DeleteCRecordWithThreeVectors(CRecordWithThreeVectors* p)                       // emission helper, not a retail function
{
    delete p;
}


#include <malloc.h>
int main()
{
    CRecordWithThreeVectors* p = new CRecordWithThreeVectors;
    p->Key = 1;
    p->First._M_start = (long*)malloc(16); p->Second._M_start = 0; p->Third._M_start = (long*)malloc(32);
    bool ok = _heapchk() == _HEAPOK;
    DeleteCRecordWithThreeVectors(p);
    DeleteCRecordWithThreeVectors(0);
    ok = ok && _heapchk() == _HEAPOK;
    if (ok) std::printf("004425c0_TEST PASS\n");
    return 0;
}