#pragma optimize("s",on)
// Name fix (NF387, T3 nf381 0x0044C0C0): the base at 0x0044C0C0 is CSubDefClassBase (retail RTTI CDragonActionHoverDef : CSubDefClassBase; ??0CSubDefClassBase@@QAE@XZ in the Ego_r and FableWin definition_manager.obj; class key V in both name sets); was the made-up CBase_0044c0c0.
// CDragonActionHoverDef::Alloc() -- factory: new CDragonActionHoverDef with the default constructor inlined over CSubDefClassBase()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044FEC7, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call CSubDefClassBase::CSubDefClassBase ; mov dword ptr [esi],offset CDragonActionHoverDef::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof(CDragonActionHoverDef) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// CSubDefClassBase() @ 0x0044C0C0, then this class's vtable 0x01232814 from rtti-vtables.tsv).
// One of 10 bodies of this shape.
class CSubDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x2c - 4];    // sizeof(CSubDefClassBase) = sizeof(CDragonActionHoverDef) = 0x2c
    CSubDefClassBase();                           // 0x0044C0C0
};

class CDragonActionHoverDef : public CSubDefClassBase { public:
    virtual void v00();
    CDragonActionHoverDef() : CSubDefClassBase() { }
};

CDragonActionHoverDef* CDragonActionHoverDef_Alloc()
{
    return new CDragonActionHoverDef;
}