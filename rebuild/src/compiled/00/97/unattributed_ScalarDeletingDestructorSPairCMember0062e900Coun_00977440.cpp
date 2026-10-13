// SPairCMember_0062e900CountedPointer_00977440::`scalar deleting destructor'(unsigned int) -- non-virtual: CCountedPointer at +0x114 released inline, CMember_0062e900 at +0 destroyed out of line, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977440, oracle length 96 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0x118] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+0x118] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0x118] ; push eax ; call operator delete ; add esp,4 ; N: mov ecx,esi ; mov dword ptr [esi+0x114],0 ; mov dword ptr [esi+0x118],0 ; call CMember_0062e900::~CMember_0062e900 ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic pair {CMember_0062e900, CCountedPointer} emitted by a delete-expression: the
// implicit destructor is inlined -- the counted pointer (declared second, destroyed first)
// releases its block (destroy function @ slot +4, flag-tested operator delete @ 0x00BFE9BC)
// and nulls its fields, then the CMember_0062e900 member's destructor @ 0x0062E900 runs; bit 0 of the
// flag argument selects the delete of the object itself. The manifest label is propagated.
// One of 9 bodies of this shape.
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CMember_0062e900 {
    unsigned char _body[0x114];
    ~CMember_0062e900();                             // 0x0062E900
};

struct SPairCMember_0062e900CountedPointer_00977440 {
    CMember_0062e900 first;                          // +0
    CCountedPointer second;                // +0x114
};

// The delete-expression that makes the compiler emit SPairCMember_0062e900CountedPointer_00977440::`scalar deleting destructor'.
void Destroy_SPairCMember_0062e900CountedPointer_00977440(SPairCMember_0062e900CountedPointer_00977440* object)
{
    delete object;
}