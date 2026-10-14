// CVectorTriple_00675b10::~CVectorTriple_00675b10() -- free the POD vectors at +0x28 and +0x18, destroy the object at +0xc out of line, then destroy the object vector at +0 (its range destroy, then the block freed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675B10, length 77 bytes.
// Callees: free @ 0x00BFEA14 three times, the member destructor @ 0x006753B0, the range destroy @ 0x00675850 (`__fastcall`: first, last, the allocator tag homed in a
// dead frame byte). Shape: a one-word frame (`push ecx`) for the tag; members destroyed in reverse declaration order. The chunk label is propagated; the class is
// named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

struct SEntry_00675b10 { unsigned char _body[8]; };
void __fastcall DestroyRange_00675850(SEntry_00675b10* first, SEntry_00675b10* last, const SScalarTag& tag);   // 0x00675850

struct CRetailVector_00675b10 {                       // VC6 vector of objects: destroy the range, free the block
    SEntry_00675b10* m_pFirst;
    SEntry_00675b10* m_pLast;
    SEntry_00675b10* m_pEnd;
    ~CRetailVector_00675b10()
    {
        DestroyRange_00675850(m_pFirst, m_pLast, SScalarTag());
        if (m_pFirst)
            free(m_pFirst);
    }
};

struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };

struct CMember_006753b0 {
    unsigned char _body[0xc];
    ~CMember_006753b0();                           // 0x006753B0
};

struct CVectorTriple_00675b10 {
    CRetailVector_00675b10 m_objects;              // +0x00
    CMember_006753b0       m_member;               // +0x0c
    CRetailVector_pod      m_a;                    // +0x18
    unsigned char          _pad24[4];
    CRetailVector_pod      m_b;                    // +0x28
    ~CVectorTriple_00675b10();
};

CVectorTriple_00675b10::~CVectorTriple_00675b10()
{
}
