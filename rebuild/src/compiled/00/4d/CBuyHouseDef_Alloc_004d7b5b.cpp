#pragma optimize("s",on)
// CBuyHouseDef::Alloc() -- factory: new CBuyHouseDef with the default constructor inlined over CBase_0044c0c0()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7B5B, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x26 ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call CBase_0044c0c0::CBase_0044c0c0 ; mov dword ptr [esi],offset CBuyHouseDef::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof(CBuyHouseDef) = 0x26 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// CBase_0044c0c0() @ 0x0044C0C0, then this class's vtable 0x0123A61C from rtti-vtables.tsv).
// One of 10 bodies of this shape.
#pragma pack(push, 1)                  // retail sizeof is not a multiple of 4: keep the exact byte count
struct CBase_0044c0c0 {
    virtual void v00();
    unsigned char _body[0x26 - 4];    // sizeof(CBase_0044c0c0) = sizeof(CBuyHouseDef) = 0x26
    CBase_0044c0c0();                           // 0x0044C0C0
};

struct CBuyHouseDef : CBase_0044c0c0 {
    virtual void v00();
    CBuyHouseDef() : CBase_0044c0c0() { }
};
#pragma pack(pop)

CBuyHouseDef* Create_CBuyHouseDef()
{
    return new CBuyHouseDef;
}