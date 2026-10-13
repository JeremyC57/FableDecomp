// CRetailBlock0x30::CRetailBlock0x30(const allocator&) -- one malloc'd block of 0x30 bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00824EF0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; push 0x30 ; mov dword ptr [esi],0 ; call malloc ; mov [esi],eax ; add esp,4 ; mov eax,esi ; pop esi ; ret 4
// A container constructor taking its allocator by value (`ret 4`): the block pointer is nulled, one
// 0x30-byte block is allocated (malloc @ 0x00BFEA0E, landed-trusted) and stored. The element type
// is not recoverable from the body, so the class is named by its block size. One of 102 bodies.
extern "C" void* __cdecl malloc(unsigned int size);

struct CRetailAllocator { };

struct CRetailBlock0x30 {
    unsigned char* block;
    CRetailBlock0x30(CRetailAllocator allocator);
};

CRetailBlock0x30::CRetailBlock0x30(CRetailAllocator allocator)
    : block(0)
{
    block = (unsigned char*)malloc(0x30);
}
