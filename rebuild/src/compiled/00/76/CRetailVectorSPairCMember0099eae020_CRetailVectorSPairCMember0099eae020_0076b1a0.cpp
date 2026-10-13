// CRetailVector_SPairCMember_0099eae020::~CRetailVector_SPairCMember_0099eae020() -- destroy each 20-byte element's two CMember_0099eae0 members (+0x4 then +0, destructor @ 0x0099EAE0), then free the array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B1A0, oracle length 57 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; mov ebx,ecx ; mov esi,[ebx] ; push edi ; mov edi,[ebx+4] ; cmp esi,edi ; je F ; L: lea ecx,[esi+0x4] ; call CMember_0099eae0::~CMember_0099eae0 ; mov ecx,esi ; call CMember_0099eae0::~CMember_0099eae0 ; add esi,0x14 ; cmp esi,edi ; jne L ; F: mov ebx,[ebx] ; test ebx,ebx ; je R ; push ebx ; call free ; add esp,4 ; R: pop edi ; pop esi ; pop ebx ; ret
// Destructor of a retail vector {first, last, end} of 20-byte entries whose first two members
// are CMember_0099eae0 objects (destructor @ 0x0099EAE0): every element's implicit destructor runs
// (members in reverse declaration order), then the element array is freed (free @ 0x00BFEA14).
// `last` is cached in a named local because the loop body calls. The manifest label is
// propagated; the classes are named by shape. One of 10 bodies.
extern "C" void __cdecl free(void* block);

struct CMember_0099eae0 {
    void* m_data;
    ~CMember_0099eae0();                             // 0x0099EAE0
};

struct SPairCMember_0099eae020 {
    CMember_0099eae0 m_first;                        // +0
    CMember_0099eae0 m_second;                       // +0x4
    unsigned char _rest[0xc];
};

struct CRetailVector_SPairCMember_0099eae020 {
    SPairCMember_0099eae020* first;
    SPairCMember_0099eae020* last;
    SPairCMember_0099eae020* end;
    ~CRetailVector_SPairCMember_0099eae020();
};

CRetailVector_SPairCMember_0099eae020::~CRetailVector_SPairCMember_0099eae020()
{
    SPairCMember_0099eae020* l = last;
    for (SPairCMember_0099eae020* p = first; p != l; ++p)
        p->~SPairCMember_0099eae020();
    if (first)
        free(first);
}