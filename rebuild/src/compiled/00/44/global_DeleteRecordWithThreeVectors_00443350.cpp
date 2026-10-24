// _global::DeleteRecordWithThreeVectors(CRecordWithThreeVectors* p) -- `delete p` of the three-vector record: when p is not null, free the vectors' storage (+0x1c, +0x10, +0x04) and then the record itself
// Leaf: DeleteRecordWithThreeVectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00443350, length 66 bytes, `__fastcall` (p in ecx), plain `ret`.
// Callees: free @ 0x00BFEA14 (each STLport vector's storage), operator delete @ 0x00BFE9BC.
// Label: the catalog "GFCheckTreeNodes" (__fastcall bool(CMemoryArea*)) is propagated and wrong: the body is a null-tested
// `delete p` with an inlined implicit destructor. No code or data references (orphaned COMDAT). The record is the one of
// the landed CRecordWithThreeVectors::`scalar deleting destructor' 0x004425C0 (a dword at +0x00, then three STLport
// vectors of plain elements at +0x04 / +0x10 / +0x1C, destroyed in reverse order); its real class name is not
// recoverable from the bytes, so the landed behavioural name is reused.
// Shape: speed TU (`add esp,4` after each call), `__fastcall` free function (/Gr-style register argument).
#include <stdlib.h>
void __cdecl operator delete(void* p);             // 0x00BFE9BC

template <class T> class CPodVector {              // STLport vector of plain elements
public:
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    T* _M_end_of_storage;                           // +0x08
    ~CPodVector() { if (_M_start) free(_M_start); }
};
class CRecordWithThreeVectors {
public:
    long              Key;                          // +0x00
    CPodVector<long>  First;                        // +0x04
    CPodVector<long>  Second;                       // +0x10
    CPodVector<long>  Third;                        // +0x1c
};

void __fastcall DeleteRecordWithThreeVectors(CRecordWithThreeVectors* p)
{
    delete p;
}