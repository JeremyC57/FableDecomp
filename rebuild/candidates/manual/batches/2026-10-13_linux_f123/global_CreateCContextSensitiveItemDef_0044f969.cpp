// Create_CContextSensitiveItemDef() -- factory: new CContextSensitiveItemDef with the default constructor inlined over CBase_0044c0c0()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044F969, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x3c ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call CBase_0044c0c0::CBase_0044c0c0 ; mov dword ptr [esi],offset CContextSensitiveItemDef::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof(CContextSensitiveItemDef) = 0x3c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// CBase_0044c0c0() @ 0x0044C0C0, then this class's vtable 0x0123227C from rtti-vtables.tsv).
// One of 10 bodies of this shape.
struct CBase_0044c0c0 {
    virtual void v00();
    unsigned char _body[0x3c - 4];    // sizeof(CBase_0044c0c0) = sizeof(CContextSensitiveItemDef) = 0x3c
    CBase_0044c0c0();                           // 0x0044C0C0
};

struct CContextSensitiveItemDef : CBase_0044c0c0 {
    virtual void v00();
    CContextSensitiveItemDef() : CBase_0044c0c0() { }
};

CContextSensitiveItemDef* Create_CContextSensitiveItemDef()
{
    return new CContextSensitiveItemDef;
}
