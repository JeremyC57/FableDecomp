// CRetailAllocator_24::allocate(count, hint) -- count ? malloc(count * 24) : 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057EFF0, oracle length 28 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; je Z ; (eax *= 24) ; push eax ; call malloc ; pop ecx ; jmp R ; Z: xor eax,eax ; R: ret 8
// Retail std::allocator<T>::allocate for a 24-byte element type: returns null for a zero count,
// else one malloc'd block (malloc @ 0x00BFEA0E, landed-trusted) of count * 24 bytes. The hint
// argument and `this` are unused (`ret 8`). The element type is not recoverable from the body;
// the allocator is named by the element size. One of 10 bodies of this shape.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator_24 {
    struct element { unsigned char bytes[24]; };
    element* allocate(unsigned long count, const void* hint);
};

CRetailAllocator_24::element* CRetailAllocator_24::allocate(unsigned long count, const void* hint)
{
    return count ? (element*)malloc(count * sizeof(element)) : 0;
}
