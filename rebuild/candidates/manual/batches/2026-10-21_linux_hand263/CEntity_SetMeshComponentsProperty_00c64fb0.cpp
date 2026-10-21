// CEntity::SetMeshComponentsProperty(const SComponentLevels* levels, unsigned long size) -- validate a list of per-component level pairs (each component GUID must be one of the four known ids, both levels within -10000..0; an empty or bad list sets the owner's last error -2 and returns -3), then store each pair in its component slot and mark dirty bit 0x400000 when the owner's current component changed; returns 0
// Leaf: SetMeshComponentsProperty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C64FB0, length 448 bytes (body 0x1b5 + int3 padding), `ret 8`.
// Identity: the manifest label is the propagated autoname; the body is the audio-library property setter family of the
//   landed CEntity::SetGuidListProperty 0x00C65870 (owner +0x14 with LastError +0x3d4, -3 failure, GUIDs compared with the
//   memcmp intrinsic). The property's real name is unknown.
// Callees: none (memcmp is the `repe cmpsd` intrinsic). Data: the component GUIDs @ 0x012B9468, 0x012B9478, 0x012B9488,
//   0x012B9498 (component indices 0..3).
// Layout: local model (no PDB class): dirty flags +0x04, owner +0x14 (current component index at owner +4), the four
//   0x18-byte component records with the two levels at +0x70/+0x74 (record base +0x70).
// Shape: the entry count is `size / 24`; a validation pass, then a store pass that maps each GUID to its index by an
//   if-chain (an unknown GUID still fails there).
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const SGuid g_EffectGuid_012b9468;          // 0x012B9468
extern const SGuid g_EffectGuid_012b9478;          // 0x012B9478
extern const SGuid g_EffectGuid_012b9488;          // 0x012B9488
extern const SGuid g_EffectGuid_012b9498;          // 0x012B9498

struct SComponentLevels {                          // 0x18 bytes
    SGuid Component;                               // +0x00
    long  Level;                                   // +0x10
    long  LevelHF;                                 // +0x14
};

struct CEntityOwner {
    unsigned char _pad0[4];
    long          CurrentComponent;                // +0x04
    unsigned char _pad8[0x3d4 - 8];
    long          LastError;                       // +0x3d4
};

struct SComponentRecord {                          // 0x18 bytes
    long          Level;                           // +0x00
    long          LevelHF;                         // +0x04
    unsigned char _pad8[0x10];
};

struct CEntity {
    unsigned char    _pad0[0x4];
    unsigned long    m_dirtyFlags;                 // +0x04
    unsigned char    _pad8[0x14 - 0x8];
    CEntityOwner*    m_pOwner;                     // +0x14
    unsigned char    _pad18[0x70 - 0x18];
    SComponentRecord m_components[4];              // +0x70
    long SetMeshComponentsProperty(const SComponentLevels* levels, unsigned long size);
};

long CEntity::SetMeshComponentsProperty(const SComponentLevels* levels, unsigned long size)
{
    unsigned long count = size / sizeof(SComponentLevels);
    if (count == 0) {
        m_pOwner->LastError = -2;
        return -3;
    }
    unsigned long i;
    for (i = 0; i < count; ++i) {
        if (memcmp(&levels[i].Component, &g_EffectGuid_012b9468, sizeof(SGuid)) != 0
            && memcmp(&levels[i].Component, &g_EffectGuid_012b9478, sizeof(SGuid)) != 0
            && memcmp(&levels[i].Component, &g_EffectGuid_012b9488, sizeof(SGuid)) != 0
            && memcmp(&levels[i].Component, &g_EffectGuid_012b9498, sizeof(SGuid)) != 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Level < -10000 || levels[i].Level > 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].LevelHF < -10000 || levels[i].LevelHF > 0) {
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
        m_components[index].Level = levels[i].Level;
        m_components[index].LevelHF = levels[i].LevelHF;
        if (index == m_pOwner->CurrentComponent)
            m_dirtyFlags |= 0x400000;
    }
    return 0;
}
