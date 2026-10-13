// CCountedPointer_00657016::CCountedPointer_00657016(T*) -- adopt the object: null-checked count block with the per-type destroy function
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657138, oracle length 62 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; push esi ; mov esi,ecx ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; Z: mov dword ptr [esi+4],0 ; mov eax,esi ; pop esi ; ret 4
// Constructor of the intrusive-count smart pointer {object, count block}: stores the object,
// then for a non-null object allocates the 12-byte block {1, destroy fn 0x00657016, object}
// (operator new @ 0x00BFEA1A, null-checked), else leaves the block pointer null. Named like the
// Reset family (unique manifest label, else the destroy function). One of 15 bodies.
void __fastcall Destroy_00657016(void* object);   // 0x00657016: the per-type deleter stored in the block

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_00657016), m_pObject(object) { }
};

struct CCountedPointer_00657016 {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer_00657016(void* object);
};

CCountedPointer_00657016::CCountedPointer_00657016(void* object)
    : m_pObject(object)
{
    if (object)
        m_pCount = new SCountBlock(m_pObject);   // re-read through `this` (retail loads [esi] for the block), not the argument
    else
        m_pCount = 0;
}
