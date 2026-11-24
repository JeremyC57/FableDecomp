// CTavernGameCoinBaseDef::CTavernGameCoinBaseDef() -- base CSubDefClassBase() then this class's vtable
// Name fix (NF381): the base at 0x0044C0C0 is CSubDefClassBase (retail RTTI CTavernGameCoinBaseDef : CSubDefClassBase; ??0CSubDefClassBase@@QAE@XZ in the Ego_r and FableWin definition_manager.obj; was CShoveHaPennyRegionDef, whose own ctor is 0x007D10F0 in tc_tavern_game_shove_ha_penny.obj).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D597F, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CSubDefClassBase::CSubDefClassBase ; mov dword ptr [esi],offset CTavernGameCoinBaseDef::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0044C0C0 runs, then the compiler stores this class's vtable (0x0123D44C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
class CSubDefClassBase { public:
    virtual void v00();
    CSubDefClassBase();                           // 0x0044C0C0
};

class CTavernGameCoinBaseDef : public CSubDefClassBase { public:
    virtual void v00();                    // this class has its own vtable
    CTavernGameCoinBaseDef();
};

CTavernGameCoinBaseDef::CTavernGameCoinBaseDef()
    : CSubDefClassBase()
{
}