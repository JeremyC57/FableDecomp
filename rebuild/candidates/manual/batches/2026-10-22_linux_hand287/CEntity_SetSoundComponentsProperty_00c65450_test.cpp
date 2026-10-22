// Behaviour test for 0x00C65450: validation errors, per-component stores and the dirty bit.
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

struct SSoundComponentLevels {                     // 0x20 bytes
    SGuid Component;                               // +0x00
    int  Level;                                   // +0x10
    float Ratio;                                   // +0x14
    float Scale1;                                  // +0x18
    float Scale2;                                  // +0x1c
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
    int SetSoundComponentsProperty(const SSoundComponentLevels* levels, unsigned int size);
};

int CEntity::SetSoundComponentsProperty(const SSoundComponentLevels* levels, unsigned int size)
{
    unsigned int count = size / sizeof(SSoundComponentLevels);
    if (count == 0) {
        m_pOwner->LastError = -2;
        return -3;
    }
    unsigned int i;
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
        if (levels[i].Ratio < 0.0f || levels[i].Ratio > 1.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Scale1 < 0.0f || levels[i].Scale1 > 10.0f) {
            m_pOwner->LastError = -2;
            return -3;
        }
        if (levels[i].Scale2 < 0.0f || levels[i].Scale2 > 10.0f) {
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
        m_sound[index].Level = levels[i].Level;
        m_sound[index].Ratio = levels[i].Ratio;
        m_sound[index].Scale1 = levels[i].Scale1;
        m_sound[index].Scale2 = levels[i].Scale2;
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
    e.m_pOwner = &owner; owner.CurrentComponent = 2;
    SSoundComponentLevels l[2];
    std::memset(l, 0, sizeof(l));
    Check(e.SetSoundComponentsProperty(l, 0) == -3 && owner.LastError == -2, "empty list");
    owner.LastError = 0;
    l[0].Component = g_EffectGuid_012b9488; l[0].Level = -500; l[0].Ratio = 0.5f; l[0].Scale1 = 2.0f; l[0].Scale2 = 3.0f;
    l[1].Component = g_EffectGuid_012b9468; l[1].Level = 0; l[1].Ratio = 1.0f; l[1].Scale1 = 0.0f; l[1].Scale2 = 10.0f;
    Check(e.SetSoundComponentsProperty(l, sizeof(l)) == 0, "ok ret");
    Check(e.m_sound[2].Level == -500 && e.m_sound[2].Ratio == 0.5f && e.m_sound[2].Scale1 == 2.0f && e.m_sound[2].Scale2 == 3.0f, "store 2");
    Check(e.m_sound[0].Level == 0 && e.m_sound[0].Ratio == 1.0f && e.m_sound[0].Scale2 == 10.0f, "store 0");
    Check(e.m_dirtyFlags == 0x400000 && owner.LastError == 0, "dirty");
    e.m_dirtyFlags = 0; owner.CurrentComponent = 3;
    Check(e.SetSoundComponentsProperty(l, sizeof(l[0])) == 0 && e.m_dirtyFlags == 0, "not current");
    l[1].Scale1 = 10.5f; e.m_sound[2].Level = 7;
    Check(e.SetSoundComponentsProperty(l, sizeof(l)) == -3 && owner.LastError == -2 && e.m_sound[2].Level == 7, "bad scale, no store");
    l[1].Scale1 = 1.0f; l[1].Level = 1; owner.LastError = 0;
    Check(e.SetSoundComponentsProperty(l, sizeof(l)) == -3 && owner.LastError == -2, "bad level");
    l[1].Level = -10000; l[1].Component.Data1 = 0x9999u; owner.LastError = 0;
    Check(e.SetSoundComponentsProperty(l, sizeof(l)) == -3 && owner.LastError == -2, "bad guid");
    l[1].Component = g_EffectGuid_012b9498; l[1].Ratio = -0.1f; owner.LastError = 0;
    Check(e.SetSoundComponentsProperty(l, sizeof(l)) == -3, "bad ratio");
    l[1].Ratio = 0.0f;
    Check(e.SetSoundComponentsProperty(l, sizeof(l) + 31) == 0 && e.m_sound[3].Level == -10000, "size rounds down");
    if (!fails) std::printf("00c65450_TEST PASS\n");
    return fails;
}
