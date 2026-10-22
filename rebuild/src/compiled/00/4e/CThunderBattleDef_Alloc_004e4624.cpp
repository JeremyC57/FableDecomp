#pragma optimize("s",on)
// CThunderBattleDef::Alloc() -- factory: new CThunderBattleDef with the default constructor inlined over CBase_00430370()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4624, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x4c ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call CBase_00430370::CBase_00430370 ; mov dword ptr [esi],offset CThunderBattleDef::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof(CThunderBattleDef) = 0x4c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// CBase_00430370() @ 0x00430370, then this class's vtable 0x01242D04 from rtti-vtables.tsv).
// One of 10 bodies of this shape.
struct CBase_00430370 {
    virtual void v00();
    unsigned char _body[0x4c - 4];    // sizeof(CBase_00430370) = sizeof(CThunderBattleDef) = 0x4c
    CBase_00430370();                           // 0x00430370
};

struct CThunderBattleDef : CBase_00430370 {
    virtual void v00();
    CThunderBattleDef() : CBase_00430370() { }
};

CThunderBattleDef* CThunderBattleDef_Alloc()
{
    return new CThunderBattleDef;
}