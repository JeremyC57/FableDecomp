// CThingDeadCreature::CreateFromDef(const CThingBaseDef& def) -- copy one flag bit of the definition (+0x41 bit 2) into the thing's flag byte (+0x91 bit 6)
// Leaf: CreateFromDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835AA0, length 32 bytes.
// Callees: none.
// Identity: CThingDeadCreature vtable 0x01272B04 slot 15, which is CThing::CreateFromDef in the CThing vtable
// (0x004CA930); Ego_r.pdb has `?CreateFromDef@CThingDeadCreature@@MAEXABVCThingBaseDef@@@Z` (the catalog's
// "vfunc_15" is unnamed). No base CreateFromDef call is made.
// Layout (retail, local model): the thing's flag byte at +0x91 (bit 0 = dead, as in the landed IsThingTurncoated
// model; bit 6 written here) and the definition's flag byte at +0x41 (bit 2 read here); the bit names are descriptive.
// Shape: a one-bit `unsigned char` bitfield assignment between two bitfields: `mov dl,[def+41h] ; shl dl,4 ; xor dl,al ;
// and dl,40h ; xor al,dl` (VC's merge of the shifted source bit into the destination byte).
struct CThingBaseDef {
    unsigned char _pad00[0x41];
    unsigned char m_41_0 : 2;                   // +0x41
    unsigned char m_inheritedFlag : 1;          // +0x41 bit 2
    unsigned char m_41_3 : 5;
};

struct CThingDeadCreature {
    unsigned char _pad00[0x91];
    unsigned char m_91_0 : 6;                   // +0x91 (bit 0 = dead)
    unsigned char m_inheritedFlag : 1;          // +0x91 bit 6
    unsigned char m_91_7 : 1;
    void CreateFromDef(const CThingBaseDef& def);
};

void CThingDeadCreature::CreateFromDef(const CThingBaseDef& def)
{
    m_inheritedFlag = def.m_inheritedFlag;
}