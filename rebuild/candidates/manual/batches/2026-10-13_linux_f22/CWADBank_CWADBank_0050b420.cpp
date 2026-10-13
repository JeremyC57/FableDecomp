// CWADBank::CWADBank() -- base CBase_009d5f80() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0050B420, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_009d5f80::CBase_009d5f80 ; mov dword ptr [esi],offset CWADBank::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x009D5F80 runs, then the compiler stores this class's vtable (0x012442DC
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CBase_009d5f80 {
    virtual void v00();
    CBase_009d5f80();                           // 0x009D5F80
};

struct CWADBank : CBase_009d5f80 {
    virtual void v00();                    // this class has its own vtable
    CWADBank();
};

CWADBank::CWADBank()
    : CBase_009d5f80()
{
}
