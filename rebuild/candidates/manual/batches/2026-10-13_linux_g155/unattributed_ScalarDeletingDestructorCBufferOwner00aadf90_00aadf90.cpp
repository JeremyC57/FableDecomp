// CBufferOwner_00aadf90::`scalar deleting destructor'(unsigned int) -- non-virtual: inlined `if (m_buffer) free(m_buffer)` (+0xc), then delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AADF90, oracle length 41 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0xc] ; test eax,eax ; je K ; push eax ; call free ; add esp,4 ; K: test byte ptr [esp+8],1 ; je R ; push esi ; call operator delete ; add esp,4 ; R: mov eax,esi ; pop esi ; ret 4
// VC7.1 emits a (non-virtual) scalar deleting destructor for a non-polymorphic class with a
// non-trivial destructor whenever a translation unit applies `delete` to it: here the inlined
// destructor releases the malloc'd buffer at +0xc with free @ 0x00BFEA14 when non-null, then
// the flag-tested operator delete @ 0x00BFE9BC frees the object. The class is named from the
// manifest label only when unique. One of 9 bodies of this shape.
extern "C" void __cdecl free(void* block);

struct CBufferOwner_00aadf90 {
    unsigned char _pad[0xc];
    void* m_buffer;                        // +0xc
    ~CBufferOwner_00aadf90() { if (m_buffer) free(m_buffer); }
};

// The delete-expression that makes the compiler emit CBufferOwner_00aadf90::`scalar deleting destructor'.
void Destroy_CBufferOwner_00aadf90(CBufferOwner_00aadf90* object)
{
    delete object;
}
