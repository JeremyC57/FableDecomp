// CBriarRoseDef::operator=(const CBriarRoseDef& other) -- the memberwise def assignment: the CDefClassBase part, then every CBriarRoseDef member in declaration order; returns *this
// Leaf: ??4CBriarRoseDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454183, 87 bytes, thiscall `ret 4`; size-optimised def TU.
// Identity (proven): CBriarRoseDef::operator=; evidence: its only reference is the 5-byte `jmp` at 0x0045417E (the tail-calling virtual in slot 19 of
//   vtable 0x012329B4, RTTI .?AVCBriarRoseDef@@; as the landed CHeroExperienceDef::operator= 0x004F914A), it copies exactly the CBriarRoseDef members
//   (offsets and sizes asserted against Ego_r.pdb, at their PDB offsets), and the PDB name sets hold ??4CBriarRoseDef@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CThingBuildingDef::operator=" is propagated (family H055: the six H052 defs' assignments, each its own COMDAT).
// Callee: CDefClassBase::operator= 0x00431F10 (copies PDefManager +0x1C, GlobalIndex +0x20 and the three flag bits at +0x24).
// Layout: CDefClassBase is 0x25 bytes (pack(1) in the PDB, members from +0x28); local classes -- the generated engine headers flatten the
//   bases (no base operator= to call), Review at landing.
// Shape: the compiler-generated assignment written out in declaration order; float members copy through eax as integer moves.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x012329B4: slot 0 0x00450003 (implicit inline dtor); real overrides from the def vtable scan
class CBriarRoseDef : public CDefClassBase {                       // vtable 0x012329B4
public:
    float SecondsToTeleport;                           // +0x28
    float SecondsToBeIntangible;                       // +0x2C
    float SecondsToChargeDrainLife;                    // +0x30
    long GuranteedImpostersHitBeforeAllowedToHitBriar; // +0x34
    long NumberOfHitsToKillImposters;                  // +0x38
    long EnvironmentThemeForIntangibleMode;            // +0x3C
    float SecondsForIntangibleEnvironmentThemeTransitionIn; // +0x40
    float SecondsForIntangibleEnvironmentThemeTransitionOut; // +0x44
    float SecondsBetweenWillUse;                       // +0x48
    float SecondsBetweenTeleportAway;                  // +0x4C
    float DistanceForcePushThreshold;                  // +0x50
    CBriarRoseDef& operator=(const CBriarRoseDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00452F8E
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x0045417E
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C653
};

CBriarRoseDef& CBriarRoseDef::operator=(const CBriarRoseDef& other)
{
    CDefClassBase::operator=(other);
    SecondsToTeleport = other.SecondsToTeleport;
    SecondsToBeIntangible = other.SecondsToBeIntangible;
    SecondsToChargeDrainLife = other.SecondsToChargeDrainLife;
    GuranteedImpostersHitBeforeAllowedToHitBriar = other.GuranteedImpostersHitBeforeAllowedToHitBriar;
    NumberOfHitsToKillImposters = other.NumberOfHitsToKillImposters;
    EnvironmentThemeForIntangibleMode = other.EnvironmentThemeForIntangibleMode;
    SecondsForIntangibleEnvironmentThemeTransitionIn = other.SecondsForIntangibleEnvironmentThemeTransitionIn;
    SecondsForIntangibleEnvironmentThemeTransitionOut = other.SecondsForIntangibleEnvironmentThemeTransitionOut;
    SecondsBetweenWillUse = other.SecondsBetweenWillUse;
    SecondsBetweenTeleportAway = other.SecondsBetweenTeleportAway;
    DistanceForcePushThreshold = other.DistanceForcePushThreshold;
    return *this;
}