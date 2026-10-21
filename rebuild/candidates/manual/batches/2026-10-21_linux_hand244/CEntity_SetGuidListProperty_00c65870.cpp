// CEntity::SetGuidListProperty(const SGuid* guids, unsigned long size) -- validate a one- or two-entry GUID list property (every GUID must be one of the five known effect ids or GUID_NULL; an empty list is rejected with the owner's last error -2 and result -3), then store the entries at +0x1a0/+0x1b0 and mark dirty bit 0x400000 when they changed; returns 0
// Leaf: SetGuidListProperty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C65870, length 400 bytes (with int3 padding; body 0x188), `ret 8`.
// Identity: the manifest label "CEntity::SetPhysicsCollisionProperty" is propagated; the body belongs to the EAX /
//   DirectSound-style property block of the landed audio-library setters CEntity::SetVectorProperty_XY 0x00C647B0 and
//   CLight::SetSlotEnabled 0x00C630D0 (same owner +0x14 / LastError +0x3d4 / -3 failure, GUIDs compared with the memcmp
//   intrinsic). The property's real name is unknown: the name here is descriptive.
// Callees: none (memcmp is the `repe cmpsd` intrinsic). Data: the GUIDs @ 0x012B9468 {C4D79F1E-...}, 0x012B9478
//   {08C00E96-...}, 0x012B9488 {1D433B88-...}, 0x012B9498 {EFFF08EA-...}, 0x012B9458 {F317866D-...}, 0x012B9438 (all zero).
// Layout: local model (no PDB class): dirty flags +0x04, owner +0x14, the two list entries +0x1a0/+0x1b0.
// Shape: the entry count is `size >> 4` (unsigned); the validation loop's index lives in the dead `size` slot and the
//   `this` pointer in the `push ecx` slot (ebp is the intrinsic's zero register); the stores are plain 16-byte copies.
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const SGuid g_EffectGuid_012b9468;          // 0x012B9468
extern const SGuid g_EffectGuid_012b9478;          // 0x012B9478
extern const SGuid g_EffectGuid_012b9488;          // 0x012B9488
extern const SGuid g_EffectGuid_012b9498;          // 0x012B9498
extern const SGuid g_EffectGuid_012b9458;          // 0x012B9458
extern const SGuid g_NullGuid_012b9438;            // 0x012B9438

struct CEntityOwner {
    unsigned char _pad[0x3d4];
    long          LastError;                       // +0x3d4
};

struct CEntity {
    unsigned char       _pad0[0x4];
    unsigned long       m_dirtyFlags;              // +0x04
    unsigned char       _pad8[0x14 - 0x8];
    CEntityOwner*       m_pOwner;                  // +0x14
    unsigned char       _pad18[0x1a0 - 0x18];
    SGuid               m_guids[2];                // +0x1a0
    long SetGuidListProperty(const SGuid* guids, unsigned long size);
};

long CEntity::SetGuidListProperty(const SGuid* guids, unsigned long size)
{
    unsigned long count = size >> 4;
    if (count == 0) {
        m_pOwner->LastError = -2;
        return -3;
    }
    for (unsigned long i = 0; i < count; ++i) {
        if (memcmp(&guids[i], &g_EffectGuid_012b9468, sizeof(SGuid)) != 0
            && memcmp(&guids[i], &g_EffectGuid_012b9478, sizeof(SGuid)) != 0
            && memcmp(&guids[i], &g_EffectGuid_012b9488, sizeof(SGuid)) != 0
            && memcmp(&guids[i], &g_EffectGuid_012b9498, sizeof(SGuid)) != 0
            && memcmp(&guids[i], &g_EffectGuid_012b9458, sizeof(SGuid)) != 0
            && memcmp(&guids[i], &g_NullGuid_012b9438, sizeof(SGuid)) != 0) {
            m_pOwner->LastError = -2;
            return -3;
        }
    }
    if (count == 1) {
        if (memcmp(&m_guids[0], &guids[0], sizeof(SGuid)) != 0) {
            m_guids[0] = guids[0];
            m_dirtyFlags |= 0x400000;
        }
    }
    else if (count == 2) {
        if (memcmp(&m_guids[0], &guids[0], sizeof(SGuid)) != 0 || memcmp(&m_guids[1], &guids[1], sizeof(SGuid)) != 0) {
            m_guids[0] = guids[0];
            m_guids[1] = guids[1];
            m_dirtyFlags |= 0x400000;
        }
    }
    return 0;
}
