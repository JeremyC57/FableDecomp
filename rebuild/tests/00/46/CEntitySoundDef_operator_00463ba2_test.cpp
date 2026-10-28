#include <cstdio>
// CEntitySoundDef::operator=(const CEntitySoundDef&) -- def copy: CDefClassBase part, sound map, identifier string and two scalars
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

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

class CSoundMap { public:                      // two CVectorMaps (SoundMap, CriteriaMap), 0x20 bytes in retail
    void* m_soundMap[4];                // CVectorMap<unsigned long, CSoundPair>: vector + sorted flag
    void* m_criteriaMap[4];             // CVectorMap<unsigned long, CDefString>
    CSoundMap& operator=(const CSoundMap& other);   // 0x0045CFAD
};

class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CEntitySoundDef : public CDefClassBase { public:
    CSoundMap SoundMap;                 // +0x28
    CCharString SoundIdentifier;        // +0x48
    float VoicePitchOverride;           // +0x4c
    long AnimCriteriaClipDistance;      // +0x50
    CEntitySoundDef& operator=(const CEntitySoundDef& other);
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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
const CCharString& CCharString::operator=(const CCharString& other) { m_pStringData = other.m_pStringData; return *this; }
CSoundMap& CSoundMap::operator=(const CSoundMap& other) { for (int i = 0; i < 4; ++i) { m_soundMap[i] = other.m_soundMap[i]; m_criteriaMap[i] = other.m_criteriaMap[i]; } return *this; }

int main()
{
    static CEntitySoundDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.SoundMap.m_soundMap[1] = (void*)&a;
    b.SoundMap.m_criteriaMap[3] = (void*)&b;
    b.SoundIdentifier.m_pStringData = (void*)&b;
    b.VoicePitchOverride = 1.25f;
    b.AnimCriteriaClipDistance = 40;
    CEntitySoundDef* r = &(a = b);
    if (r != &a) { std::printf("00463ba2_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "00463ba2"); return 1; }
    if (!(a.SoundMap.m_soundMap[1] == b.SoundMap.m_soundMap[1])) { std::printf("00463ba2_TEST FAIL\n"); return 1; }
    if (!(a.SoundMap.m_criteriaMap[3] == b.SoundMap.m_criteriaMap[3])) { std::printf("00463ba2_TEST FAIL\n"); return 1; }
    if (!(a.SoundIdentifier.m_pStringData == b.SoundIdentifier.m_pStringData)) { std::printf("00463ba2_TEST FAIL\n"); return 1; }
    if (!(a.VoicePitchOverride == b.VoicePitchOverride)) { std::printf("00463ba2_TEST FAIL\n"); return 1; }
    if (!(a.AnimCriteriaClipDistance == b.AnimCriteriaClipDistance)) { std::printf("00463ba2_TEST FAIL\n"); return 1; }
    std::printf("00463ba2_TEST PASS\n");
    return 0;
}