// CIntelligentPointer::CIntelligentPointer() -- base CBaseIntelligentPointer() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D67E3, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov dword ptr [esi],offset CIntelligentPointer::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00A01B10 runs, then the compiler stores this class's vtable (0x0123E7E0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                    // this class has its own vtable
    CIntelligentPointer();
};

CIntelligentPointer::CIntelligentPointer()
    : CBaseIntelligentPointer()
{
}