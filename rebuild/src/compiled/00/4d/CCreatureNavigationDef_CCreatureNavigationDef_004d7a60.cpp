// CCreatureNavigationDef::CCreatureNavigationDef() -- base CShoveHaPennyRegionDef() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7A60, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CShoveHaPennyRegionDef::CShoveHaPennyRegionDef ; mov dword ptr [esi],offset CCreatureNavigationDef::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0044C0C0 runs, then the compiler stores this class's vtable (0x0123E98C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CShoveHaPennyRegionDef { public:
    virtual void v00();
    CShoveHaPennyRegionDef();                           // 0x0044C0C0
};

class CCreatureNavigationDef : public CShoveHaPennyRegionDef { public:
    virtual void v00();                    // this class has its own vtable
    CCreatureNavigationDef();
};

CCreatureNavigationDef::CCreatureNavigationDef()
    : CShoveHaPennyRegionDef()
{
}