// SPairCMember_0099eae0CountedPointer_00a0a4d0::`scalar deleting destructor'(unsigned int) -- non-virtual: CCountedPointer at +0x4 released inline, CCharString at +0 destroyed out of line, flag-tested delete
// Naming (naming wave 22, ccs): the string type is the PDB CCharString (was CMember_0099eae0); name-only.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A4D0, oracle length 81 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0x8] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+0x8] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0x8] ; push eax ; call operator delete ; add esp,4 ; N: mov ecx,esi ; mov dword ptr [esi+0x4],0 ; mov dword ptr [esi+0x8],0 ; call CCharString::~CCharString ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic pair {CCharString, CCountedPointer} emitted by a delete-expression: the
// implicit destructor is inlined -- the counted pointer (declared second, destroyed first)
// releases its block (destroy function @ slot +4, flag-tested operator delete @ 0x00BFE9BC)
// and nulls its fields, then the CCharString member's destructor @ 0x0099EAE0 runs; bit 0 of the
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

class CCharString { public:
    unsigned char _body[0x4];
    ~CCharString();                             // 0x0099EAE0
};

struct SPairCMember_0099eae0CountedPointer_00a0a4d0 {
    CCharString first;                          // +0
    CCountedPointer second;                // +0x4
};

// The delete-expression that makes the compiler emit SPairCMember_0099eae0CountedPointer_00a0a4d0::`scalar deleting destructor'.
void Destroy_SPairCMember_0099eae0CountedPointer_00a0a4d0(SPairCMember_0099eae0CountedPointer_00a0a4d0* object)
{
    delete object;
}