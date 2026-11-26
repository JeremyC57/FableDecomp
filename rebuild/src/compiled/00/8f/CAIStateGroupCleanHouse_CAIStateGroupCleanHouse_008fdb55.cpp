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
// Naming (NF460, queue nf458): the base is CAIStateGroup_Clean (RTTI .?AVCAIStateGroup_CleanHouse@@ lists it as the direct base at offset 0; its
//   constructor 0x008FDB27 is ??0CAIStateGroup_Clean@@QAE@XZ in FableWin.pdb and that row's self). Was the stand-in
//   N_008fdb27::CInterestingToVillagersDef. Same bytes.
class CAIStateGroup_Clean { public:
    virtual void v00();
    CAIStateGroup_Clean();                                  // 0x008FDB27
};

struct CAIStateGroup_CleanHouse : CAIStateGroup_Clean {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_CleanHouse();
};

CAIStateGroup_CleanHouse::CAIStateGroup_CleanHouse()
    : CAIStateGroup_Clean()
{
}