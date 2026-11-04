// C3DAnimationUtil::C3DAnimationUtil() -- base CEnvironmentSpellEffectInterface() then this class's vtable
// Vendor naming wave V1 (one address, one name): 0x00C6CA20 ??0CVtable_012bed7c@@QAE@XZ (was ??0CEnvironmentSpellEffectInterface@@QAE@XZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55070, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface ; mov dword ptr [esi],offset C3DAnimationUtil::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00C6CA20 runs, then the compiler stores this class's vtable (0x012B8A64
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
class CVtable_012bed7c { public:
    virtual void v00();
    CVtable_012bed7c();                           // 0x00C6CA20
};

struct C3DAnimationUtil : CVtable_012bed7c {
    virtual void v00();                    // this class has its own vtable
    C3DAnimationUtil();
};

C3DAnimationUtil::C3DAnimationUtil()
    : CVtable_012bed7c()
{
}