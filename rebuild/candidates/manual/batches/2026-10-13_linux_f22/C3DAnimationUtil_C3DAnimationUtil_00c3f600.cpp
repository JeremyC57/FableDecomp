// C3DAnimationUtil::C3DAnimationUtil() -- base CBase_00c5b1a0() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F600, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00c5b1a0::CBase_00c5b1a0 ; mov dword ptr [esi],offset C3DAnimationUtil::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00C5B1A0 runs, then the compiler stores this class's vtable (0x012B5CD0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CBase_00c5b1a0 {
    virtual void v00();
    CBase_00c5b1a0();                           // 0x00C5B1A0
};

struct C3DAnimationUtil : CBase_00c5b1a0 {
    virtual void v00();                    // this class has its own vtable
    C3DAnimationUtil();
};

C3DAnimationUtil::C3DAnimationUtil()
    : CBase_00c5b1a0()
{
}
