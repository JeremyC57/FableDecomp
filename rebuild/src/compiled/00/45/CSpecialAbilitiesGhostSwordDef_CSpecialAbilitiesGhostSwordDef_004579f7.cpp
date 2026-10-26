// CSpecialAbilitiesGhostSwordDef::CSpecialAbilitiesGhostSwordDef() -- base CParentDefClassBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004579F7, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CParentDefClassBase::CParentDefClassBase ; mov dword ptr [esi],offset CSpecialAbilitiesGhostSwordDef::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00430370 runs, then the compiler stores this class's vtable (0x012344D4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CParentDefClassBase { public:
    virtual void v00();
    CParentDefClassBase();                           // 0x00430370
};

class CSpecialAbilitiesGhostSwordDef : public CParentDefClassBase { public:
    virtual void v00();                    // this class has its own vtable
    CSpecialAbilitiesGhostSwordDef();
};

CSpecialAbilitiesGhostSwordDef::CSpecialAbilitiesGhostSwordDef()
    : CParentDefClassBase()
{
}