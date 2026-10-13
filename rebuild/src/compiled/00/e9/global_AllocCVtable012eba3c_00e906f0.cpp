// Alloc_CVtable_012eba3c(void*, void*) -- factory: new CVtable_012eba3c(first, second) with the constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E906F0, oracle length 54 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; push edi ; push 0x50 ; mov edi,edx ; mov ebx,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; mov ecx,esi ; call CBase_00cb8110::CBase_00cb8110 ; mov [esi+0x40],edi ; pop edi ; mov [esi+0x44],ebx ; mov dword ptr [esi],offset CVtable_012eba3c::`vftable' ; mov eax,esi ; pop esi ; pop ebx ; ret ; N: pop edi ; pop esi ; xor eax,eax ; pop ebx ; ret
// A __fastcall factory (ecx = first, edx = second): allocates sizeof(CVtable_012eba3c) = 0x50 bytes
// (operator new @ 0x00BFEA1A, landed-trusted), null-checks the block and runs the inlined
// two-argument constructor: base CBase_00cb8110() @ 0x00CB8110, second argument stored at +0x40,
// first at +0x44, vtable 0x0X12EBA3C (named through rtti-vtables.tsv). The constructor body
// is the same shape the out-of-line family lands as CVtable_012eba3c::CVtable_012eba3c. One of 75 bodies.
struct CBase_00cb8110 {
    virtual void v00();
    CBase_00cb8110();                           // 0x00CB8110
};

struct CVtable_012eba3c : CBase_00cb8110 {
    unsigned char _pad[0x3c];
    void* m_second;                           // +0x40
    void* m_first;                           // +0x44
    unsigned char _tail[0x8];
    virtual void v00();
    CVtable_012eba3c(void* first, void* second) : CBase_00cb8110(), m_second(second), m_first(first) { }
};

CVtable_012eba3c* __fastcall Alloc_CVtable_012eba3c(void* first, void* second)
{
    return new CVtable_012eba3c(first, second);
}