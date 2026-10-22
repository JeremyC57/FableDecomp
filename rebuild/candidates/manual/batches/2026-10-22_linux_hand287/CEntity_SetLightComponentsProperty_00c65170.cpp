// CEntity::SetLightComponentsProperty(const SAllComponentLevels* levels, unsigned long size) -- validate a list of per-component combined settings (known component GUID; sound level/ratio/scales, VFX level/ratio and the two mesh levels in range; a bad entry sets the owner's last error -2 and returns -3), then store each entry's sound, VFX and mesh values in its component's records and mark dirty bit 0x400000 when the owner's current component changed; returns 0
// Leaf: SetLightComponentsProperty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C65170, length 736 bytes (+ int3 padding), `ret 8`.
// Identity: the manifest label is the propagated autoname; the body is the audio-library property setter family of the
//   landed CEntity::SetVFXComponentsProperty 0x00C65690 / SetMeshComponentsProperty 0x00C64FB0 (owner +0x14 with LastError
//   +0x3d4, -3 failure, GUIDs compared with the memcmp intrinsic). The property's real name is unknown.
// Callees: none (memcmp is the `repe cmpsd` intrinsic). Data: the component GUIDs @ 0x012B9468, 0x012B9478, 0x012B9488,
//   0x012B9498 (component indices 0..3); float pool 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, 10.0f @ 0x01237F0C.
// Layout: local model (no PDB class) shared with the landed siblings: dirty flags +0x04, owner +0x14 (current component
//   index at owner +4), mesh records +0x70 (0x18 each), sound records +0xd0 (0x20 each), VFX records +0x150 (0x18 each).
// Shape: 48-byte entries (`size / 48`, the 0xAAAAAAAB multiply); unlike the single-record siblings an empty list is not an
//   error. Validation order: sound, VFX, mesh fields; the store order is the same.
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const SGuid g_EffectGuid_012b9468;          // 0x012B9468
extern const SGuid g_EffectGuid_012b9478;          // 0x012B9478
extern const SGuid g_EffectGuid_012b9488;          // 0x012B9488
extern const SGuid g_EffectGuid_012b9498;          // 0x012B9498

struct CEntityOwner {
    unsigned char _pad0[4];
    long          CurrentComponent;                // +0x04
    unsigned char _pad8[0x3d4 - 8];
    long          LastError;                       // +0x3d4
};

struct SMeshRecord {                               // 0x18 bytes
    long          Level;                           // +0x00
    long          LevelHF;                         // +0x04
    unsigned char _pad8[0x10];
};
struct SSoundRecord {                              // 0x20 bytes
    long          Level;                           // +0x00
    float         Ratio;                           // +0x04
    float         Scale1;                          // +0x08
    float         Scale2;                          // +0x0c
    unsigned char _pad10[0x10];
};
struct SVFXRecord {                                // 0x18 bytes
    long          Level;                           // +0x00
    float         Ratio;                           // +0x04
    unsigned char _pad8[0x10];
};

struct SAllComponentLevels {                       // 0x30 bytes
    SGuid Component;                               // +0x00
    long  MeshLevel;                               // +0x10
    long  MeshLevelHF;                             // +0x14
    long  SoundLevel;                              // +0x18
    float SoundRatio;                              // +0x1c
    float SoundScale1;                             // +0x20
    float SoundScale2;                             // +0x24
    long  VFXLevel;                                // +0x28
    float VFXRatio;                                // +0x2c
};

struct CEntity {
    unsigned char    _pad0[0x4];
    unsigned long    m_dirtyFlags;                 // +0x04
    unsigned char    _pad8[0x14 - 0x8];
    CEntityOwner*    m_pOwner;                     // +0x14
    unsigned char    _pad18[0x70 - 0x18];
    SMeshRecord      m_mesh[4];                    // +0x70
    SSoundRecord     m_sound[4];                   // +0xd0
    SVFXRecord       m_vfx[4];                     // +0x150
    long SetLightComponentsProperty(const SAllComponentLevels* levels, unsigned long size);
};

long CEntity::SetLightComponentsProperty(const SAllComponentLevels* levels, unsigned long size)
{
    unsigned long count = size / sizeof(SAllComponentLevels);
    unsigned long i;
    for (i = 0; i < count; ++i) {
        if (memcmp(&levels[i].Component, &g_EffectGuid_012b9468, sizeof(SGuid)) != 0
            && memcmp(&levels[i].Component, &g_EffectGuid_012b9478, sizeof(SGuid)) != 0
            && memcmp(&levels[i].Component, &g_EffectGuid_012b9488, sizeof(SGuid)) != 0
            && memcmp(&levels[i].Component, &g_EffectGuid_012b9498, sizeof(SGuid)) != 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].SoundLevel < -10000 || levels[i].SoundLevel > 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].SoundRatio < 0.0f || levels[i].SoundRatio > 1.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].SoundScale1 < 0.0f || levels[i].SoundScale1 > 10.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].SoundScale2 < 0.0f || levels[i].SoundScale2 > 10.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].VFXLevel < -10000 || levels[i].VFXLevel > 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].VFXRatio < 0.0f || levels[i].VFXRatio > 1.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].MeshLevel < -10000 || levels[i].MeshLevel > 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].MeshLevelHF < -10000 || levels[i].MeshLevelHF > 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
    }
    for (i = 0; i < count; ++i) {
        long index;
        if (memcmp(&levels[i].Component, &g_EffectGuid_012b9468, sizeof(SGuid)) == 0)
            index = 0;
        else if (memcmp(&levels[i].Component, &g_EffectGuid_012b9478, sizeof(SGuid)) == 0)
            index = 1;
        else if (memcmp(&levels[i].Component, &g_EffectGuid_012b9488, sizeof(SGuid)) == 0)
            index = 2;
        else if (memcmp(&levels[i].Component, &g_EffectGuid_012b9498, sizeof(SGuid)) == 0)
            index = 3;
        else {
            m_pOwner->LastError = -2;
            return -3;
        }
        m_sound[index].Level = levels[i].SoundLevel;
        m_sound[index].Ratio = levels[i].SoundRatio;
        m_sound[index].Scale1 = levels[i].SoundScale1;
        m_sound[index].Scale2 = levels[i].SoundScale2;
        m_vfx[index].Level = levels[i].VFXLevel;
        m_vfx[index].Ratio = levels[i].VFXRatio;
        m_mesh[index].Level = levels[i].MeshLevel;
        m_mesh[index].LevelHF = levels[i].MeshLevelHF;
        if (index == m_pOwner->CurrentComponent)
            m_dirtyFlags |= 0x400000;
    }
    return 0;
}
