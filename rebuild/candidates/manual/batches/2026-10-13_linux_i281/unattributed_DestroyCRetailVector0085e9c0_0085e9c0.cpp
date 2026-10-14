// CRetailVector_0085e9c0::Destroy() -- free each element's owned block (at +0x0 of the 12-byte element, free @ 0x00BFEA14) from first to last, then the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085E9C0, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; (pad) ; L: mov eax,[esi+0x0] ; test eax,eax ; je N ; push eax ; call free ; add esp,4 ; N: add esi,0xc ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Vector teardown for 12-byte elements that own one heap block each (at +0x0): the end
// pointer is cached before the loop, every non-null block is freed, then the buffer itself.
// Labels are propagated; names are descriptive. One of 6 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SElement {
    void* m_pData;                         // +0x0 owned block
    unsigned char _rest[0x8];
};

struct CRetailVector_0085e9c0 {
    SElement* first;
    SElement* last;
    void Destroy_CRetailVector_0085e9c0();
};

void CRetailVector_0085e9c0::Destroy_CRetailVector_0085e9c0()
{
    SElement* p = first;
    SElement* end = last;
    for (; p != end; ++p) {
        if (p->m_pData)
            free(p->m_pData);
    }
    if (first)
        free(first);
}
