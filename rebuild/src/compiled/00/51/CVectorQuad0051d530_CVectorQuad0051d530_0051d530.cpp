// CVectorQuad_0051d530::~CVectorQuad_0051d530() -- free the POD vectors at +0x24, +0x18 and +0xc, then destroy the object vector at +0 (its range destroy, then the block freed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051D530, length 85 bytes.
// Callees: free @ 0x00BFEA14 four times, the range destroy @ 0x0051CD80 (`__fastcall`: first, last, the allocator tag by reference, its temporary homed in a dead frame byte `lea eax,[esp+7]`).
// Shape: a one-word frame (`push ecx`) for the tag; members destroyed in reverse declaration order; the last block pointer is loaded into esi. The chunk label
// is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

struct SEntry_0051d530 { unsigned char _body[8]; };
void __fastcall DestroyRange_0051cd80(SEntry_0051d530* first, SEntry_0051d530* last, const SScalarTag& tag);   // 0x0051CD80

struct CRetailVector_0051d530 {                       // VC6 vector of objects: destroy the range, free the block
    SEntry_0051d530* m_pFirst;
    SEntry_0051d530* m_pLast;
    SEntry_0051d530* m_pEnd;
    ~CRetailVector_0051d530()
    {
        DestroyRange_0051cd80(m_pFirst, m_pLast, SScalarTag());
        if (m_pFirst)
            free(m_pFirst);
    }
};

struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };

struct CVectorQuad_0051d530 {
    CRetailVector_0051d530 m_objects;              // +0x00
    CRetailVector_pod      m_a;                    // +0x0c
    CRetailVector_pod      m_b;                    // +0x18
    CRetailVector_pod      m_c;                    // +0x24
    ~CVectorQuad_0051d530();
};

CVectorQuad_0051d530::~CVectorQuad_0051d530()
{
}