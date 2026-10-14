// CChunkedFileChunk::~CChunkedFileChunk() -- empty destructor: stores the vtable (0x012B957C) and tail-jumps to the base destructor @ 0x00C74450
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C69740, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [ecx], 0x12b957c ; jmp 0xc74450
// Tiny-body shape `DtorToBase`: a derived class with an empty destructor over a base whose
// destructor is out of line -- the derived vtable is stored (a relocation) and the base
// destructor is tail-called on the same `this`. The class comes from the manifest label.
struct CBase_00c69740 {
    virtual void v00();
    ~CBase_00c69740();                            // 0x00C74450
};

struct CChunkedFileChunk : CBase_00c69740 {              // vtable 0x012B957C
    ~CChunkedFileChunk();
};

CChunkedFileChunk::~CChunkedFileChunk()
{
}