// C3DAnimationUtil::C3DAnimationUtil() -- base CIcicleData() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D970, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CIcicleData::CIcicleData ; mov dword ptr [esi],offset C3DAnimationUtil::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00C42D60 runs, then the compiler stores this class's vtable (0x012B5AB0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CIcicleData {
    virtual void v00();
    CIcicleData();                           // 0x00C42D60
};

struct C3DAnimationUtil : CIcicleData {
    virtual void v00();                    // this class has its own vtable
    C3DAnimationUtil();
};

C3DAnimationUtil::C3DAnimationUtil()
    : CIcicleData()
{
}
