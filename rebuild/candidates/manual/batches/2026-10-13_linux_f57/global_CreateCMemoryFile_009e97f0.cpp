// Create_CMemoryFile(CMemoryFile*) -- factory: new CMemoryFile(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E97F0, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x24 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CMemoryFile::CMemoryFile ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CMemoryFile) = 0x24 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x009E99C0 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CMemoryFile;

struct CMemoryFile {
    unsigned char _body[0x24];       // sizeof(CMemoryFile) = 0x24
    CMemoryFile(CMemoryFile* owner);                   // 0x009E99C0
};

CMemoryFile* __fastcall Create_CMemoryFile(CMemoryFile* owner)
{
    return new CMemoryFile(owner);
}
