// CRecord24Vector::resize(unsigned long newSize) -- STLport's one-argument vector::resize: forwards to resize(newSize, T()) with a value-initialised 24-byte record temporary, then destroys the temporary (its inner vector and buffer)
// Grade: DIFFER(104v104) -- register choice only: the inner vector's first pointer is loaded into ecx and copied to edi (`mov ecx,[esp+14h] ... mov edi,ecx`); retail loads edi and copies to ecx
// Leaf: ?resize@CRecord24Vector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051D870, length 104 bytes (`ret 4`, `this` passed straight through in ecx).
// Label correction: the catalog's "CLODDynamicBuildInfo::~CLODDynamicBuildInfo" is propagated: the body never stores a vtable
// or touches a CLODDynamicBuildInfo (Ego_r.pdb: 20 bytes, no vector); it calls the two-argument resize @ 0x0051D800 (the
// manifest's vector<CTCInventoryMap::CUIMiniMapMarker>::resize, 24-byte elements: `imul 2AAAAAABh ; sar edx,2`).
// Callees: vector::resize(size_type, const T&) @ 0x0051D800, the inner element range destructor _Destroy(first, last,
// allocator) @ 0x00519EF0 (`__fastcall`; manifest "_Move_backward<pair<unsigned long,CSoundPair>*>" is propagated),
// free @ 0x00BFEA14.
// Model: the 24-byte record is { buffer +0 (freed), two words +4/+8, an STLport vector +0xc/+0x10/+0x14 }, all zeroed by
// its constructor; the record type is not recoverable, so it is named by size. Shape: the six zero stores share esi; the
// temporary dies in reverse member order (inner vector: its first pointer held in edi, range destroy with the allocator tag homed in the dead argument
// slot, then `free(first)`; then the buffer); free's null tests are explicit.
extern "C" void __cdecl free(void* p);            // 0x00BFEA14
struct SAllocTag { SAllocTag() {} };
struct SInnerElement;
void __fastcall DestroyRange(SInnerElement* first, SInnerElement* last, const SAllocTag& tag);   // 0x00519EF0
struct CInnerVector {
    SInnerElement* m_pFirst;
    SInnerElement* m_pLast;
    SInnerElement* m_pEnd;
    CInnerVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
    ~CInnerVector()
    {
        SInnerElement* first = m_pFirst;
        DestroyRange(first, m_pLast, SAllocTag());
        if (first)
            free(first);
    }
};
struct CBuffer {
    void* m_p;
    CBuffer() : m_p(0) {}
    ~CBuffer()
    {
        if (m_p)
            free(m_p);
    }
};
struct SRecord24 {                                 // 24 bytes
    CBuffer      m_buffer;                         // +0x00
    long         m_04;                             // +0x04
    long         m_08;                             // +0x08
    CInnerVector m_items;                          // +0x0c
    SRecord24() : m_04(0), m_08(0) {}
};
class CRecord24Vector {                            // STLport vector<SRecord24>
public:
    SRecord24* m_pFirst;
    SRecord24* m_pLast;
    SRecord24* m_pEnd;
    void resize(unsigned long newSize, const SRecord24& value);   // 0x0051D800
    void resize(unsigned long newSize);
};

void CRecord24Vector::resize(unsigned long newSize)
{
    resize(newSize, SRecord24());
}
