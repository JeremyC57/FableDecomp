// CEntitySoundDef::operator=(const CEntitySoundDef&) -- def copy: CDefClassBase part, sound map, identifier string and two scalars
// Leaf: ??4CEntitySoundDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00463BA2, 57 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), CSoundMap::operator= @ 0x0045CFAD, CCharString::operator= @ 0x0099EFB0.
// Identification: PDB CEntitySoundDef = CDefClassBase + SoundMap (CSoundMap, 0x28 in the PDB, 0x20 in retail: its two
//   vector-backed maps lose 4 bytes each) at +0x28, SoundIdentifier (CCharString) +0x48, VoicePitchOverride (float) +0x4c,
//   AnimCriteriaClipDistance +0x50.
// Shape: the implicit memberwise assignment written out (the DDK out-lines it): two out-of-line member assignments
//   through `lea`, then the two dwords through eax.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CSoundMap { public:                      // two CVectorMaps (SoundMap, CriteriaMap), 0x20 bytes in retail
    void* m_soundMap[4];                // CVectorMap<unsigned long, CSoundPair>: vector + sorted flag
    void* m_criteriaMap[4];             // CVectorMap<unsigned long, CDefString>
    CSoundMap& operator=(const CSoundMap& other);   // 0x0045CFAD
};

class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

// def vtable 0x01233A44: slot 0 0x0045CE06 (implicit inline dtor); real overrides from the def vtable scan
class CEntitySoundDef : public CDefClassBase { public:
    CSoundMap SoundMap;                 // +0x28
    CCharString SoundIdentifier;        // +0x48
    float VoicePitchOverride;           // +0x4c
    long AnimCriteriaClipDistance;      // +0x50
    CEntitySoundDef& operator=(const CEntitySoundDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004702C8
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00463B9D
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004564B1
};

CEntitySoundDef& CEntitySoundDef::operator=(const CEntitySoundDef& other)
{
    CDefClassBase::operator=(other);
    SoundMap = other.SoundMap;
    SoundIdentifier = other.SoundIdentifier;
    VoicePitchOverride = other.VoicePitchOverride;
    AnimCriteriaClipDistance = other.AnimCriteriaClipDistance;
    return *this;
}