// Behaviour test for 0x00C65170: validation errors, per-component stores and the dirty bit.
#include <cstdio>
#include <cstring>
struct SGuid { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
static const SGuid g_EffectGuid_012b9468 = {0x1001u, 1, 2, {0,0,0,0,0,0,0,1}};
static const SGuid g_EffectGuid_012b9478 = {0x1002u, 1, 2, {0,0,0,0,0,0,0,2}};
static const SGuid g_EffectGuid_012b9488 = {0x1003u, 1, 2, {0,0,0,0,0,0,0,3}};
static const SGuid g_EffectGuid_012b9498 = {0x1004u, 1, 2, {0,0,0,0,0,0,0,4}};

struct CEntityOwner {
    unsigned char _pad0[4];
    int          CurrentComponent;                // +0x04
    unsigned char _pad8[0x3d4 - 8];
    int          LastError;                       // +0x3d4
};

struct SMeshRecord {                               // 0x18 bytes
    int          Level;                           // +0x00
    int          LevelHF;                         // +0x04
    unsigned char _pad8[0x10];
};
struct SSoundRecord {                              // 0x20 bytes
    int          Level;                           // +0x00
    float         Ratio;                           // +0x04
    float         Scale1;                          // +0x08
    float         Scale2;                          // +0x0c
    unsigned char _pad10[0x10];
};
struct SVFXRecord {                                // 0x18 bytes
    int          Level;                           // +0x00
    float         Ratio;                           // +0x04
    unsigned char _pad8[0x10];
};

struct SAllComponentLevels {                       // 0x30 bytes
    SGuid Component;                               // +0x00
    int  MeshLevel;                               // +0x10
    int  MeshLevelHF;                             // +0x14
    int  SoundLevel;                              // +0x18
    float SoundRatio;                              // +0x1c
    float SoundScale1;                             // +0x20
    float SoundScale2;                             // +0x24
    int  VFXLevel;                                // +0x28
    float VFXRatio;                                // +0x2c
};

struct CEntity {
    unsigned char    _pad0[0x4];
    unsigned int m_dirtyFlags;                 // +0x04
    unsigned char    _pad8[0x14 - 0x8];
    CEntityOwner*    m_pOwner;                     // +0x14
    unsigned char    _pad18[0x70 - 0x18];
    SMeshRecord      m_mesh[4];                    // +0x70
    SSoundRecord     m_sound[4];                   // +0xd0
    SVFXRecord       m_vfx[4];                     // +0x150
    int SetLightComponentsProperty(const SAllComponentLevels* levels, unsigned int size);
};

int CEntity::SetLightComponentsProperty(const SAllComponentLevels* levels, unsigned int size)
{
    unsigned int count = size / sizeof(SAllComponentLevels);
    unsigned int i;
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
        int index;
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

static int fails = 0;
static void Check(int cond, const char* what) { if (!cond) { std::printf("FAIL %s\n", what); ++fails; } }
int main()
{
    static CEntityOwner owner; static CEntity e;
    std::memset(&owner, 0, sizeof(owner)); std::memset(&e, 0, sizeof(e));
    e.m_pOwner = &owner; owner.CurrentComponent = 1;
    SAllComponentLevels l[1];
    std::memset(l, 0, sizeof(l));
    Check(e.SetLightComponentsProperty(l, 0) == 0 && owner.LastError == 0, "empty list ok");
    l[0].Component = g_EffectGuid_012b9478; l[0].MeshLevel = -1; l[0].MeshLevelHF = -2; l[0].SoundLevel = -3;
    l[0].SoundRatio = 0.25f; l[0].SoundScale1 = 4.0f; l[0].SoundScale2 = 5.0f; l[0].VFXLevel = -6; l[0].VFXRatio = 0.75f;
    Check(e.SetLightComponentsProperty(l, sizeof(l)) == 0, "ok ret");
    Check(e.m_sound[1].Level == -3 && e.m_sound[1].Ratio == 0.25f && e.m_sound[1].Scale1 == 4.0f && e.m_sound[1].Scale2 == 5.0f, "sound");
    Check(e.m_vfx[1].Level == -6 && e.m_vfx[1].Ratio == 0.75f, "vfx");
    Check(e.m_mesh[1].Level == -1 && e.m_mesh[1].LevelHF == -2, "mesh");
    Check(e.m_dirtyFlags == 0x400000, "dirty");
    l[0].MeshLevelHF = 5; e.m_mesh[1].LevelHF = 9;
    Check(e.SetLightComponentsProperty(l, sizeof(l)) == -3 && owner.LastError == -2 && e.m_mesh[1].LevelHF == 9, "bad meshHF");
    l[0].MeshLevelHF = 0; l[0].VFXRatio = 1.5f; owner.LastError = 0;
    Check(e.SetLightComponentsProperty(l, sizeof(l)) == -3 && owner.LastError == -2, "bad vfx ratio");
    l[0].VFXRatio = 1.0f; l[0].Component.Data2 = 77; owner.LastError = 0;
    Check(e.SetLightComponentsProperty(l, sizeof(l)) == -3 && owner.LastError == -2, "bad guid");
    if (!fails) std::printf("00c65170_TEST PASS\n");
    return fails;
}
