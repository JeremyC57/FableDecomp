// CBedDef::CBedDef() -- base CSubDefClassBase(), this vtable, four CDefString members set to -1
// Name fix (NF381): the base at 0x0044C0C0 is CSubDefClassBase (retail RTTI CBedDef : CSubDefClassBase; ??0CSubDefClassBase@@QAE@XZ in the Ego_r and FableWin definition_manager.obj; was CShoveHaPennyRegionDef, whose own ctor is 0x007D10F0 in tc_tavern_game_shove_ha_penny.obj).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7A25, length 33 bytes.
// Callees: CSubDefClassBase::CSubDefClassBase @ 0x0044C0C0 (Ego_r / FableWin definition_manager.obj; a
// def base shared by the whole 0x0044xxxx def module).
// Vtable 0x0123E8BC = CBedDef (rtti-vtables.tsv; the byte-identical CCoopSpiritDef constructor is @ 0x0044C343,
// vtable 0x01232214). Members from the PDB: GetInBedAnimName +0x28, SleepInBedAnimName +0x2c,
// GetUpFromBedAnimName +0x30, GetUpFromBedFastAnimName +0x34 (CDefString, -1 = no name).
// Shape: the vtable store precedes the four `-1` stores because each is a member object with an
// inline constructor (a CDefString, "no name" = -1); one `or eax,-1` register feeds all four.
#pragma optimize("s", on)
class CSubDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                  // +0x04..+0x27
    CSubDefClassBase();                   // 0x0044C0C0
};

class CDefString { public:                             // a name index into the def string table
    long m_index;
    CDefString() : m_index(-1) { }
};

class CBedDef : public CSubDefClassBase { public:   // vtable 0x0123E8BC
    CDefString GetInBedAnimName;        // +0x28
    CDefString SleepInBedAnimName;      // +0x2c
    CDefString GetUpFromBedAnimName;    // +0x30
    CDefString GetUpFromBedFastAnimName; // +0x34
    virtual void v00();
    CBedDef();
};

CBedDef::CBedDef()
    : CSubDefClassBase(), GetInBedAnimName(), SleepInBedAnimName(), GetUpFromBedAnimName(), GetUpFromBedFastAnimName()
{
}