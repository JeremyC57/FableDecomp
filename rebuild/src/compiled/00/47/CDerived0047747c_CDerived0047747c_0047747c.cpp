// CDerived_0047747c::~CDerived_0047747c() -- destroy the vector of 0x88-byte polymorphic records at +0x48 (destroy range, free the block), free the POD vector's block at +0x3c, then the base destructor as the tail jump
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047747C, length 47 bytes.
// Leaf: ??1CDerived_0047747c
// Callees: the `__fastcall` range destroy @ 0x004774B9 (first in ecx, last in edx; slot 0 with 0 on each 0x88-byte record;
// the manifest's "vector<CIntelligentPointer<CThing const>>::_Destroy" is propagated), the vector's free @ 0x004774AB
// (`if (first) free(first)`; "CActiveFile::OnReadFinished" is propagated), free @ 0x00BFEA14, the base destructor
// @ 0x00430300 (frees +0x29, stores 0x01230BA0, tail-jumps 0x009FC550). Called by the scalar deleting destructor @ 0x00477460
// (and from 0x00479974 / 0x00479B89). The chunk label "CTCAssassinRush::FinishMove" is propagated and rejected; named by address.
// Shape: the size-optimised module (`pop ecx` after free): `#pragma optimize("s", on)`. No vtable store of its own:
// `__declspec(novtable)` most-derived class with an empty destructor. Under "s" the record vector's inline destructor is
// emitted out of line unless `__forceinline`; `last` is loaded before `first` (right-to-left register arguments).
#pragma optimize("s", on)
void __cdecl free(void* block);                // 0x00BFEA14

struct SRecord_0047747c;                       // 0x88-byte polymorphic element

void __fastcall DestroyRange_004774b9(SRecord_0047747c* first, SRecord_0047747c* last);   // 0x004774B9

struct CRecordVector {                         // retail std::vector (12 bytes)
    SRecord_0047747c* m_first;
    SRecord_0047747c* m_last;
    SRecord_0047747c* m_end;
    void Free();                               // 0x004774AB
    __forceinline ~CRecordVector() { DestroyRange_004774b9(m_first, m_last); Free(); }
};

struct CPodVector {                            // retail std::vector of PODs
    void* m_first;
    void* m_last;
    void* m_end;
    ~CPodVector() { if (m_first) free(m_first); }
};

struct CBase_0047747c {
    virtual void v00();
    unsigned char _body[0x38];                 // +0x04..+0x3c
    ~CBase_0047747c();                         // 0x00430300
};

struct __declspec(novtable) CDerived_0047747c : CBase_0047747c {
    CPodVector    m_values;                    // +0x3c
    CRecordVector m_records;                   // +0x48
    ~CDerived_0047747c();
};

CDerived_0047747c::~CDerived_0047747c()
{
}