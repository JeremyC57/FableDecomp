// CFeatDef::CFeatDef(const CFeatDef&) -- copy constructor: base CDefClassBase copy, own vtable 0x012FB778, then the fifteen feat fields in declaration order
// Leaf: ??0CFeatDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F2D270, length 117 bytes (+ int3 padding).
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C (as in the landed CBlurredAttitudesDef copy 0x0067C6D0).
// Twin of CFeatDef::operator= 0x00F2D200 (same member copy run).
// Layout: PDB CFeatDef (sizeof 0x64): 0x28-byte CDefClassBase, then FeatName +0x28, Verb +0x2c, TimeLimit +0x30,
// TargetNumber +0x34, GoldReward +0x38, XPReward +0x3c, ItemReward +0x40, NoBlocking +0x44, KN_AttackType +0x48,
// KN_Perfect +0x4c, KN_CreatureType +0x50, GF_FromRegion +0x54, GF_ToRegion +0x58, GF_NoTeleporting +0x5c, CO_ItemName +0x60.
// Shape: memberwise initialiser list (the vtable store follows the base call), every member, each word through eax/ecx/edx in rotation, the bools through cl/al.
// No engine header exists for CFeatDef: modelled locally from struct_layouts_egor.tsv.

struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                                            // +0x04..+0x28
    CDefClassBase(const CDefClassBase& other);                             // 0x0046644C
};

struct CDefString {
    long TablePos;
};

struct CFeatDef : CDefClassBase {                                         // vtable 0x012FB778
    CDefString FeatName;                                                  // +0x28
    CDefString Verb;                                                      // +0x2c
    float      TimeLimit;                                                 // +0x30
    long       TargetNumber;                                              // +0x34
    long       GoldReward;                                                // +0x38
    long       XPReward;                                                  // +0x3c
    CDefString ItemReward;                                                // +0x40
    bool       NoBlocking;                                                // +0x44
    long       KN_AttackType;                                             // +0x48 EFeatAttackType
    bool       KN_Perfect;                                                // +0x4c
    long       KN_CreatureType;                                           // +0x50
    CDefString GF_FromRegion;                                             // +0x54
    CDefString GF_ToRegion;                                               // +0x58
    bool       GF_NoTeleporting;                                          // +0x5c
    CDefString CO_ItemName;                                               // +0x60
    virtual void v00();
    CFeatDef(const CFeatDef& other);
};

CFeatDef::CFeatDef(const CFeatDef& other)
    : CDefClassBase(other)
    , FeatName(other.FeatName)
    , Verb(other.Verb)
    , TimeLimit(other.TimeLimit)
    , TargetNumber(other.TargetNumber)
    , GoldReward(other.GoldReward)
    , XPReward(other.XPReward)
    , ItemReward(other.ItemReward)
    , NoBlocking(other.NoBlocking)
    , KN_AttackType(other.KN_AttackType)
    , KN_Perfect(other.KN_Perfect)
    , KN_CreatureType(other.KN_CreatureType)
    , GF_FromRegion(other.GF_FromRegion)
    , GF_ToRegion(other.GF_ToRegion)
    , GF_NoTeleporting(other.GF_NoTeleporting)
    , CO_ItemName(other.CO_ItemName)
{
}
