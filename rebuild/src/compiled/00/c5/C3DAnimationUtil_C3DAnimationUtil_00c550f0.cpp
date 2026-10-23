// C3DAnimationUtil::C3DAnimationUtil() -- base CResourceBank() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C550F0, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CResourceBank::CResourceBank ; mov dword ptr [esi],offset C3DAnimationUtil::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00C6D9D0 runs, then the compiler stores this class's vtable (0x012B8AB4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
namespace N_00c6d9d0 { struct CResourceBank {
    virtual void v00();
    CResourceBank();                           // 0x00C6D9D0
}; } using N_00c6d9d0::CResourceBank;

struct C3DAnimationUtil : CResourceBank {
    virtual void v00();                    // this class has its own vtable
    C3DAnimationUtil();
};

C3DAnimationUtil::C3DAnimationUtil()
    : CResourceBank()
{
}