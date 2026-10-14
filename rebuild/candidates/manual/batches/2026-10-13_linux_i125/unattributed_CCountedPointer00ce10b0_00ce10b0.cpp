// CCountedPointer_00ce10b0::CCountedPointer_00ce10b0(T*) -- store the object; when non-null allocate the 12-byte count block (count 1, destroy routine @ 0x00CE1100, the object re-read from the member), else null the block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE10B0, oracle length 78 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[object] ; test eax,eax ; push esi ; mov esi,ecx ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je F ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; F: xor eax,eax ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; Z: mov dword ptr [esi+4],0 ; mov eax,esi ; pop esi ; ret 4
// Counted-pointer constructor from a raw pointer: the object is stored first, then the count
// block is `new`'d inline for a non-null object (VC's null check on the non-throwing
// allocation leaves the field null on failure) with count 1, the type-erased destroy routine
// @ 0x00CE1100 and the object re-read from the member (the allocation call could have
// changed it). The manifest label names one instantiation; the object type is opaque. One of
// 8 bodies of this shape.
struct CObject;

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object);
};

void __fastcall CountedDestroy(void* object);   // 0x00CE1100

inline SCountBlock::SCountBlock(void* object)
    : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object)
{
}

struct CCountedPointer_00ce10b0 {
    CObject* m_pObject;                    // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer_00ce10b0(CObject* object);
};

CCountedPointer_00ce10b0::CCountedPointer_00ce10b0(CObject* object)
{
    m_pObject = object;
    if (object)
        m_pCount = new SCountBlock(m_pObject);
    else
        m_pCount = 0;
}
