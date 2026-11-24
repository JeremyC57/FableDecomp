#pragma optimize("s",on)
// Name fix (NF387, T3 nf381 0x0044C0C0): the base at 0x0044C0C0 is CSubDefClassBase (retail RTTI CCameraCollisionDef : CSubDefClassBase; ??0CSubDefClassBase@@QAE@XZ in the Ego_r and FableWin definition_manager.obj; class key V in both name sets); was the made-up CBase_0044c0c0.
// CCameraCollisionDef::Alloc() -- factory: new CCameraCollisionDef with the default constructor inlined over CSubDefClassBase()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D9465, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call CSubDefClassBase::CSubDefClassBase ; mov dword ptr [esi],offset CCameraCollisionDef::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof(CCameraCollisionDef) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// CSubDefClassBase() @ 0x0044C0C0, then this class's vtable 0x0123DFBC from rtti-vtables.tsv).
// One of 10 bodies of this shape.
class CSubDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x2c - 4];    // sizeof(CSubDefClassBase) = sizeof(CCameraCollisionDef) = 0x2c
    CSubDefClassBase();                           // 0x0044C0C0
};

class CCameraCollisionDef : public CSubDefClassBase { public:
    virtual void v00();
    CCameraCollisionDef() : CSubDefClassBase() { }
};

CCameraCollisionDef* CCameraCollisionDef_Alloc()
{
    return new CCameraCollisionDef;
}