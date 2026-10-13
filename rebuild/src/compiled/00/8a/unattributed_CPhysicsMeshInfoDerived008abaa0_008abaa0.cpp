// CPhysicsMeshInfo_Derived_008abaa0::~CPhysicsMeshInfo_Derived_008abaa0() -- releases the CCountedPointer member at +0xa8 (count block destroy + delete at zero, fields nulled), then CPhysicsMeshInfo::~CPhysicsMeshInfo by tail call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ABAA0, oracle length 75 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+0xac] ; test eax,eax ; je N ; dec dword ptr [eax] ; mov eax,[esi+0xac] ; cmp dword ptr [eax],0 ; jne N ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+0xac] ; push eax ; call operator delete ; add esp,4 ; N: mov dword ptr [esi+0xa8],0 ; mov dword ptr [esi+0xac],0 ; mov ecx,esi ; pop esi ; jmp CPhysicsMeshInfo::~CPhysicsMeshInfo
// Implicit/empty destructor: the CCountedPointer member at +0xa8 is released inline (decrement
// the block's count, at zero call its destroy function with the object in ecx and delete the
// block through the flag-tested operator delete @ 0x00BFE9BC, then null both fields), and
// the base class destructor @ 0x00693EF0 runs by tail call. The manifest label is propagated.
// One of 12 bodies of this shape.
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

struct CPhysicsMeshInfo_Derived_008abaa0 : CPhysicsMeshInfo {
    CCountedPointer m_pointer;             // +0xa8
    ~CPhysicsMeshInfo_Derived_008abaa0();
};

CPhysicsMeshInfo_Derived_008abaa0::~CPhysicsMeshInfo_Derived_008abaa0()
{
}