// CSummonerDef::operator=(const CSummonerDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CSummonerDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045426C, 99 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CSummonerDef::Copy 0x00454267; the 13 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CSummonerDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M036, representative 0x0045426C): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CSummonerDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x01232AEC: slot 0 0x004500D5 (implicit inline dtor); real overrides from the def vtable scan
class CSummonerDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     LightningRangeForAutomaticUse;     // +0x28
    float                     LightningPercentChanceForRandomUse; // +0x2c
    float                     LightningPercentChanceForBoastUse; // +0x30
    float                     LightningRandomChanceTestSeconds;  // +0x34
    float                     LightningSecondsToWaitBeforeReuse; // +0x38
    float                     ChargeRangeNear;                   // +0x3c
    float                     ChargeRangeFar;                    // +0x40
    float                     ChargeRangeAttackInterrupt;        // +0x44
    float                     ChargeSecondsToWaitBeforeReuse;    // +0x48
    long                      SummonerDeathExplosion;            // +0x4c
    long                      LightningOrb;                      // +0x50
    long                      LightningOrbAttackFireheart;       // +0x54
    float                     LightningOrbLifeSeconds;           // +0x58
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004530DC
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C695
};
void CSummonerDef::Copy(const CDefClassBase* other)
{
    *this = *(const CSummonerDef*)other;
}