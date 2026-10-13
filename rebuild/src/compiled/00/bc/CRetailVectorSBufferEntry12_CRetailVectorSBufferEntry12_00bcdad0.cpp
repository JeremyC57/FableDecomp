// CRetailVector_SBufferEntry12::~CRetailVector_SBufferEntry12() -- destroy each 12-byte element (free its buffer when set), then free the element array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCDAD0, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; L: mov eax,[esi] ; test eax,eax ; je N ; push eax ; call free ; add esp,4 ; N: add esi,0xc ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Destructor of a retail vector {first, last, end} of 12-byte entries whose first member is a
// malloc'd buffer: every element's inline destructor frees its buffer (free @ 0x00BFEA14,
// null-checked), then the element array itself is freed. `last` is cached in a named local
// because the loop body calls. The manifest label (`Vector_DeleteAllElements_Impl`) is
// propagated; the classes are named by shape. One of 13 bodies.
extern "C" void __cdecl free(void* block);

struct SBufferEntry12 {
    void* m_pBuffer;                       // +0
    unsigned char _rest[0x8];
    ~SBufferEntry12() { if (m_pBuffer) free(m_pBuffer); }
};

struct CRetailVector_SBufferEntry12 {
    SBufferEntry12* first;
    SBufferEntry12* last;
    SBufferEntry12* end;
    ~CRetailVector_SBufferEntry12();
};

CRetailVector_SBufferEntry12::~CRetailVector_SBufferEntry12()
{
    SBufferEntry12* l = last;
    for (SBufferEntry12* p = first; p != l; ++p)
        p->~SBufferEntry12();
    if (first)
        free(first);
}