// CVectorTriple_00a851e0::~CVectorTriple_00a851e0() -- destroy the object vectors at +0x18 and +0xc (each: the range destroy, then the block freed), then free the POD vector at +0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A851E0, length 86 bytes.
// Callees: the range destroys @ 0x00A83420 and @ 0x00A827D0 (`__fastcall`: first, last, the allocator tag by reference, its temporary homed in a dead frame byte `lea eax,[esp+7]`), free @
// 0x00BFEA14 three times. Shape: a one-word frame (`push ecx`) for the tag; members destroyed in reverse declaration order; the last block pointer is loaded into
// esi. The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

struct SEntryA_00a851e0 { unsigned char _body[8]; };
struct SEntryB_00a851e0 { unsigned char _body[8]; };
void __fastcall DestroyRangeA_00a83420(SEntryA_00a851e0* first, SEntryA_00a851e0* last, const SScalarTag& tag);   // 0x00A83420
void __fastcall DestroyRangeB_00a827d0(SEntryB_00a851e0* first, SEntryB_00a851e0* last, const SScalarTag& tag);   // 0x00A827D0

struct CRetailVectorA_00a851e0 {
    SEntryA_00a851e0* m_pFirst; SEntryA_00a851e0* m_pLast; SEntryA_00a851e0* m_pEnd;
    ~CRetailVectorA_00a851e0() { DestroyRangeA_00a83420(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); }
};
struct CRetailVectorB_00a851e0 {
    SEntryB_00a851e0* m_pFirst; SEntryB_00a851e0* m_pLast; SEntryB_00a851e0* m_pEnd;
    ~CRetailVectorB_00a851e0() { DestroyRangeB_00a827d0(m_pFirst, m_pLast, SScalarTag()); if (m_pFirst) free(m_pFirst); }
};
struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };

struct CVectorTriple_00a851e0 {
    CRetailVector_pod       m_pods;                // +0x00
    CRetailVectorB_00a851e0 m_b;                   // +0x0c
    CRetailVectorA_00a851e0 m_a;                   // +0x18
    ~CVectorTriple_00a851e0();
};

CVectorTriple_00a851e0::~CVectorTriple_00a851e0()
{
}
