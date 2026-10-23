// CAIStateGroup_CleanHouse::CAIStateGroup_CleanHouse() -- base CInterestingToVillagersDef() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDB55, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CInterestingToVillagersDef::CInterestingToVillagersDef ; mov dword ptr [esi],offset CAIStateGroup_CleanHouse::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008FDB27 runs, then the compiler stores this class's vtable (0x01289A9C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
namespace N_008fdb27 { struct CInterestingToVillagersDef {
    virtual void v00();
    CInterestingToVillagersDef();                           // 0x008FDB27
}; } using N_008fdb27::CInterestingToVillagersDef;

struct CAIStateGroup_CleanHouse : CInterestingToVillagersDef {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_CleanHouse();
};

CAIStateGroup_CleanHouse::CAIStateGroup_CleanHouse()
    : CInterestingToVillagersDef()
{
}