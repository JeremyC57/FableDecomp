// CThingSwitchDef::CThingSwitchDef() -- base CThingBaseDef() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454EFF, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CThingBaseDef::CThingBaseDef ; mov dword ptr [esi],offset CThingSwitchDef::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x006772D0 runs, then the compiler stores this class's vtable (0x012332CC
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CThingBaseDef { public:
    virtual void v00();
    CThingBaseDef();                           // 0x006772D0
};

class CThingSwitchDef : public CThingBaseDef { public:
    virtual void v00();                    // this class has its own vtable
    CThingSwitchDef();
};

CThingSwitchDef::CThingSwitchDef()
    : CThingBaseDef()
{
}