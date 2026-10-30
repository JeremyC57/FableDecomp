// CKickableDef::operator=(const CKickableDef& other) -- the memberwise def assignment: the CDefClassBase part, then every CKickableDef member in declaration order; returns *this
// Leaf: ??4CKickableDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0222, 87 bytes, thiscall `ret 4`; size-optimised def TU.
// Identity (proven): CKickableDef::operator=; evidence: its only reference is the 5-byte `jmp` at 0x004E021D (the tail-calling virtual in slot 19 of
//   vtable 0x0123A7AC, RTTI .?AVCKickableDef@@; as the landed CHeroExperienceDef::operator= 0x004F914A), it copies exactly the CKickableDef members
//   (offsets and sizes asserted against Ego_r.pdb, at their PDB offsets), and the PDB name sets hold ??4CKickableDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CThingBuildingDef::operator=" is propagated (family H055: the six H052 defs' assignments, each its own COMDAT).
// Callee: CDefClassBase::operator= 0x00431F10 (copies PDefManager +0x1C, GlobalIndex +0x20 and the three flag bits at +0x24).
// Layout: CDefClassBase is 0x25 bytes (pack(1) in the PDB, members from +0x28); local classes -- the generated engine headers flatten the
//   bases (no base operator= to call), Review at landing.
// Shape: the compiler-generated assignment written out in declaration order; float members copy through eax as integer moves.
#pragma optimize("s", on)
enum EOpinionDeedType { };                                  // enumerators not needed
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0123A7AC: slot 0 0x004D7C51 (implicit inline dtor); real overrides from the def vtable scan
class CKickableDef : public CDefClassBase {                       // vtable 0x0123A7AC
public:
    float BaseDistanceToTravel;                        // +0x28
    float StrengthScaleRange;                          // +0x2C
    float SpeedScaleRange;                             // +0x30
    float BaseSpeedScaleRange;                         // +0x34
    float RandomBase;                                  // +0x38
    float RandomRange;                                 // +0x3C
    long ShotToAttach;                                 // +0x40
    EOpinionDeedType OpinionDeedType;                  // +0x44
    long OpinionDeedFaction;                           // +0x48
    EOpinionDeedType OpinionDeedTypeToEnemies;         // +0x4C
    bool IgnoreNavigationTest;                         // +0x50
    CKickableDef& operator=(const CKickableDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DDC74
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E021D
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D33C2
};

CKickableDef& CKickableDef::operator=(const CKickableDef& other)
{
    CDefClassBase::operator=(other);
    BaseDistanceToTravel = other.BaseDistanceToTravel;
    StrengthScaleRange = other.StrengthScaleRange;
    SpeedScaleRange = other.SpeedScaleRange;
    BaseSpeedScaleRange = other.BaseSpeedScaleRange;
    RandomBase = other.RandomBase;
    RandomRange = other.RandomRange;
    ShotToAttach = other.ShotToAttach;
    OpinionDeedType = other.OpinionDeedType;
    OpinionDeedFaction = other.OpinionDeedFaction;
    OpinionDeedTypeToEnemies = other.OpinionDeedTypeToEnemies;
    IgnoreNavigationTest = other.IgnoreNavigationTest;
    return *this;
}