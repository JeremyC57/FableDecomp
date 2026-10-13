// bool __stdcall LessByKeyThenAddress(const SKeyed*, const SKeyed*) -- a->m_1c != b->m_1c ? a->m_1c < b->m_1c : a < b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C248E0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[edx+0x1c] ; push esi ; mov esi,[esp+0Ch] ; mov ecx,[esi+0x1c] ; cmp eax,ecx ; jne S ; cmp edx,esi ; S: sbb eax,eax ; neg eax ; pop esi ; ret 8
// Strict weak ordering helper (__stdcall, two object pointers): compares the unsigned dword at
// +0x1c; equal keys fall back to the objects' addresses. Both branches end in the same
// `sbb/neg` materialisation of the carry flag, so the compiler shares it. The manifest has no
// label for this gap-scan body; it is named descriptively. One of 10 bodies.
struct SKeyed {
    unsigned char _pad[0x1c];
    unsigned long m_key;                   // +0x1c
};

bool __stdcall LessByKeyThenAddress_00c248e0(const SKeyed* a, const SKeyed* b)
{
    if (a->m_key != b->m_key)
        return a->m_key < b->m_key;
    return a < b;
}
