// CRecordWithThreeVectors::`scalar deleting destructor'(unsigned int flags) -- implicit destructor (free the three vectors' storage), then free the object when bit 0 is set
// Leaf: ??_GCRecordWithThreeVectors@@QAEPAXI@Z
// Retail: Fable.exe @ 0x004425C0, 73 bytes, `ret 4`; no code or data references (orphaned COMDAT). Catalog label
// CTCWound::CWound::Kill is wrong: the body is an inlined implicit destructor -- three STLport vectors of plain elements at
// +0x1c, +0x10, +0x04 (reverse order) each `if (_M_start) free(_M_start)` (0x00BFEA14), no vptr store -- then
// `if (flags & 1) operator delete(this)` (0x00BFE9BC). The class name is behavioural; the real one is not recoverable
// from the bytes (no PDB class has three vectors at +4/+0x14/+0x24 that also fits).
// Layout: one dword +0x00, then three 12-byte STLport vectors +0x04 / +0x10 / +0x1c.
// Shape: non-virtual destructor; the `delete p` helper makes VC7.1 emit ??_G (as 0x00443C50).
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