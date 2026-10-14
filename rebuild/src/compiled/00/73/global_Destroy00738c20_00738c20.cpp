// _Destroy_range<CCountedPointer*> -- release every counted pointer in [first, last) (count at +4, virtual slot 1 destroys at zero, field nulled)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00738C20, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov edi,edx ; mov esi,ecx ; cmp esi,edi ; je R ; (pad) ; L: mov ecx,[esi] ; test ecx,ecx ; je N ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: mov dword ptr [esi],0 ; N: ; add esi,0x4 ; cmp esi,edi ; jne L ; R: pop edi ; pop esi ; ret 4
// Dinkumware `_Destroy_range` over 4-byte elements: each counted pointer releases its object through CObject::Release (the count at +4, virtual slot 1 at zero) and nulls its field. Fastcall: first in ecx, last in edx; a trailing stack argument (the allocator) is unused. The manifest label is
// propagated; the element type is opaque. One of 8 bodies of this shape.
struct CObject {
    virtual void v00();
    virtual void Destroy();                // slot 1
    long m_references;                     // +4
    void Release() { if (--m_references == 0) Destroy(); }
};

struct CCountedPointer {
    CObject* m_p;
    ~CCountedPointer() { if (m_p) { m_p->Release(); m_p = 0; } }
};

void __fastcall Destroy_00738c20(CCountedPointer* first, CCountedPointer* last, long unused)
{
    for (; first != last; ++first)
        first->~CCountedPointer();
}