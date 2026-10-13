// SPairCPhysicsMeshInfoCountedPointer_008ac330::`scalar deleting destructor'(unsigned int) -- non-virtual: CCountedPointer at +0xa8 released inline, CPhysicsMeshInfo at +0 destroyed out of line, flag-tested delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC330, oracle length 96 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0xac] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+0xac] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0xac] ; push eax ; call operator delete ; add esp,4 ; N: mov ecx,esi ; mov dword ptr [esi+0xa8],0 ; mov dword ptr [esi+0xac],0 ; call CPhysicsMeshInfo::~CPhysicsMeshInfo ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// `??_G` of a NON-polymorphic pair {CPhysicsMeshInfo, CCountedPointer} emitted by a delete-expression: the
// implicit destructor is inlined -- the counted pointer (declared second, destroyed first)
// releases its block (destroy function @ slot +4, flag-tested operator delete @ 0x00BFE9BC)
// and nulls its fields, then the CPhysicsMeshInfo member's destructor @ 0x00693EF0 runs; bit 0 of the
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

struct CPhysicsMeshInfo {
    unsigned char _body[0xa8];
    ~CPhysicsMeshInfo();                             // 0x00693EF0
};

struct SPairCPhysicsMeshInfoCountedPointer_008ac330 {
    CPhysicsMeshInfo first;                          // +0
    CCountedPointer second;                // +0xa8
};

// The delete-expression that makes the compiler emit SPairCPhysicsMeshInfoCountedPointer_008ac330::`scalar deleting destructor'.
void Destroy_SPairCPhysicsMeshInfoCountedPointer_008ac330(SPairCPhysicsMeshInfoCountedPointer_008ac330* object)
{
    delete object;
}