// CThingDrainLifeShotDef::CThingDrainLifeShotDef() -- base CShoveHaPennyRegionDef() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5392, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CShoveHaPennyRegionDef::CShoveHaPennyRegionDef ; mov dword ptr [esi],offset CThingDrainLifeShotDef::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0044C0C0 runs, then the compiler stores this class's vtable (0x0123CCA4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CShoveHaPennyRegionDef { public:
    virtual void v00();
    CShoveHaPennyRegionDef();                           // 0x0044C0C0
};

class CThingDrainLifeShotDef : public CShoveHaPennyRegionDef { public:
    virtual void v00();                    // this class has its own vtable
    CThingDrainLifeShotDef();
};

CThingDrainLifeShotDef::CThingDrainLifeShotDef()
    : CShoveHaPennyRegionDef()
{
}