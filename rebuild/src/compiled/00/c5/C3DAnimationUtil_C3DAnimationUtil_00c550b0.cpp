// C3DAnimationUtil::C3DAnimationUtil() -- base CBase_00c6e760() then this class's vtable
// Vendor naming wave V1 (one address, one name): 0x00C6E760 ??0CVtable_012bedb8@@QAE@XZ (was ??0CBase_00c6e760@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C550B0, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00c6e760::CBase_00c6e760 ; mov dword ptr [esi],offset C3DAnimationUtil::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00C6E760 runs, then the compiler stores this class's vtable (0x012B8A8C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CVtable_012bedb8 {
    virtual void v00();
    CVtable_012bedb8();                           // 0x00C6E760
};

struct C3DAnimationUtil : CVtable_012bedb8 {
    virtual void v00();                    // this class has its own vtable
    C3DAnimationUtil();
};

C3DAnimationUtil::C3DAnimationUtil()
    : CVtable_012bedb8()
{
}