// C3DAnimationUtil::C3DAnimationUtil() -- base CNavigationLayer() then this class's vtable
// Leaf: ??0C3DAnimationUtil@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C573E0, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CNavigationLayer::CNavigationLayer ; mov dword ptr [esi],offset C3DAnimationUtil::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00C57780 runs, then the compiler stores this class's vtable (0x012B8E20
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
namespace N_00c57780 { struct CNavigationLayer {
    virtual void v00();
    CNavigationLayer();                           // 0x00C57780
}; } using N_00c57780::CNavigationLayer;

struct C3DAnimationUtil : CNavigationLayer {
    virtual void v00();                    // this class has its own vtable
    C3DAnimationUtil();
};

C3DAnimationUtil::C3DAnimationUtil()
    : CNavigationLayer()
{
}