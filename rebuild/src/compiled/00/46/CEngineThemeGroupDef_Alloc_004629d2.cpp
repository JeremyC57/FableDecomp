#pragma optimize("s",on)
// CEngineThemeGroupDef::Alloc() -- factory: new CEngineThemeGroupDef with the default constructor inlined over CBase_00430370()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004629D2, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x3a ; call operator new ; mov esi,eax ; test esi,esi ; pop ecx ; je N ; mov ecx,esi ; call CBase_00430370::CBase_00430370 ; mov dword ptr [esi],offset CEngineThemeGroupDef::`vftable' ; mov eax,esi ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A no-argument factory: allocates sizeof(CEngineThemeGroupDef) = 0x3a bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the inlined default constructor (base
// CBase_00430370() @ 0x00430370, then this class's vtable 0x0123512C from rtti-vtables.tsv).
// One of 10 bodies of this shape.
#pragma pack(push, 1)                  // retail sizeof is not a multiple of 4: keep the exact byte count
struct CBase_00430370 {
    virtual void v00();
    unsigned char _body[0x3a - 4];    // sizeof(CBase_00430370) = sizeof(CEngineThemeGroupDef) = 0x3a
    CBase_00430370();                           // 0x00430370
};

class CEngineThemeGroupDef : public CBase_00430370 { public:
    virtual void v00();
    CEngineThemeGroupDef() : CBase_00430370() { }
};
#pragma pack(pop)

CEngineThemeGroupDef* Create_CEngineThemeGroupDef()
{
    return new CEngineThemeGroupDef;
}