// CBuffer_00c29550::~CBuffer_00c29550() -- deletes the block at +0x4 and zeroes the three fields
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29550, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0x4] ; test eax,eax ; je K ; push eax ; call operator delete ; add esp,4 ; K: mov dword ptr [esi+0x4],0 ; mov dword ptr [esi+0x8],0 ; mov dword ptr [esi+0xc],0 ; pop esi ; ret
// Destructor of a small owning container: releases the heap block at +0x4 (operator delete
// @ 0x00BFE9BC, landed-trusted, with the compiler's null check) and clears the pointer and the
// two counters at +0x8, +0xc. The manifest labels the family `Stack<unsigned long,64>::~Stack`
// (a template instantiation; kept only when unique). One of 11 bodies of this shape.
struct CBuffer_00c29550 {
    unsigned char _pad[0x4];
    unsigned long* m_data;                 // +0x4
    unsigned long m_count;                 // +0x8
    unsigned long m_capacity;              // +0xc
    ~CBuffer_00c29550();
};

CBuffer_00c29550::~CBuffer_00c29550()
{
    if (m_data)
        delete m_data;
    m_data = 0;
    m_count = 0;
    m_capacity = 0;
}