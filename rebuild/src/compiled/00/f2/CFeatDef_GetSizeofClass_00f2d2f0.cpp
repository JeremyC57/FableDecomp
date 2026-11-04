// CFeatDef::GetSizeofClass() -- slot 20: sizeof(CFeatDef) = 0x64
// Leaf: ?GetSizeofClass@CFeatDef@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F2D2F0, 6 bytes: b864000000c3.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB778 slot 20; Anniversary RTTI CFeatDef (string-literal match, validated 257/258); PDB name set has the GetSizeofClass member of CFeatDef; slot layout from named TLC siblings; function referenced by this vtable only; body=mov eax,imm;ret.
//   ?GetSizeofClass@CFeatDef@@UAEKXZ in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??_ECFeatDef@@UAEPAXI@Z, ??1CFeatDef@@UAE@XZ.
// Was: Constant-return member: returns 0x64. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed operator= row 0x00F2D200 (rebuild/src/compiled/00/f2/CFeatDef_operator_00f2d200.cpp); only this member is graded.

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CDefString { public:
    long TablePos;
};

// def vtable 0x012FB778: slot 0 0x00F2D300 (implicit inline dtor); real overrides from the def vtable scan
class CFeatDef : public CDefClassBase { public:
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
    CFeatDef& operator=(const CFeatDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00F2D330
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00F2D680
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00F2D2F0
};

CFeatDef& CFeatDef::operator=(const CFeatDef& other)
{
    CDefClassBase::operator=(other);
    FeatName = other.FeatName;
    Verb = other.Verb;
    TimeLimit = other.TimeLimit;
    TargetNumber = other.TargetNumber;
    GoldReward = other.GoldReward;
    XPReward = other.XPReward;
    ItemReward = other.ItemReward;
    NoBlocking = other.NoBlocking;
    KN_AttackType = other.KN_AttackType;
    KN_Perfect = other.KN_Perfect;
    KN_CreatureType = other.KN_CreatureType;
    GF_FromRegion = other.GF_FromRegion;
    GF_ToRegion = other.GF_ToRegion;
    GF_NoTeleporting = other.GF_NoTeleporting;
    CO_ItemName = other.CO_ItemName;
    return *this;
}
unsigned long CFeatDef::GetSizeofClass()
{
    return sizeof(CFeatDef);
}
