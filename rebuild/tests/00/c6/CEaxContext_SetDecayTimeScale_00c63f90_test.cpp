// behaviour test for 0x00C63F90 CEaxContext::SetDecayTimeScale: EAX 4.0 reverb flag bit 0
#include <cstdio>
#include <cstring>
#include <cmath>
typedef unsigned int u32;
struct SGuid { u32 Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct EAXVECTOR { float x, y, z; };
struct EAXFXSLOTPROPERTIES { SGuid guidLoadEffect; int lVolume; int lLock; u32 ulFlags; };
struct EAXREVERBPROPERTIES {
    u32 ulEnvironment; float flEnvironmentSize; float flEnvironmentDiffusion; int lRoom; int lRoomHF; int lRoomLF;
    float flDecayTime; float flDecayHFRatio; float flDecayLFRatio; int lReflections; float flReflectionsDelay;
    EAXVECTOR vReflectionsPan; int lReverb; float flReverbDelay; EAXVECTOR vReverbPan; float flEchoTime;
    float flEchoDepth; float flModulationTime; float flModulationDepth; float flAirAbsorptionHF; float flHFReference;
    float flLFReference; float flRoomRolloffFactor; u32 ulFlags;
};
struct EAXCONTEXTPROPERTIES { SGuid guidPrimaryFXSlotID; float flDistanceFactor; float flAirAbsorptionHF; float flHFReference; };
static const SGuid EAX_REVERB_EFFECT = { 0x0cf95c8f, 0xa3cc, 0x4849, { 0xb0, 0xb6, 0x83, 0x2e, 0xcc, 0x18, 0x22, 0xdf } };
static const SGuid EAXPROPERTYID_EAX40_FXSlot0 = { 0xc4d79f1e, 0xf1ac, 0x436b, { 0xa8, 0x1d, 0xa7, 0x38, 0xe7, 0x04, 0x54, 0x69 } };
static const SGuid EAXPROPERTYID_EAX40_FXSlot1 = { 0x08c00e96, 0x74be, 0x4491, { 0x93, 0xaa, 0xe8, 0xad, 0x35, 0xa4, 0x91, 0x17 } };
static const SGuid EAXPROPERTYID_EAX40_FXSlot2 = { 0x1d433b88, 0xf0f6, 0x4637, { 0x91, 0x9f, 0x60, 0xe7, 0xe0, 0x6b, 0x5e, 0xdd } };
static const SGuid EAXPROPERTYID_EAX40_FXSlot3 = { 0xefff08ea, 0xc7d8, 0x44ab, { 0x93, 0xad, 0x6d, 0xbd, 0x5f, 0x91, 0x00, 0x64 } };
inline bool operator==(const SGuid& a, const SGuid& b) { return !std::memcmp(&a, &b, sizeof(SGuid)); }
static int IsNaN(double x) { return x != x; }

struct CEaxContext {
    int m_primarySlot;
    EAXFXSLOTPROPERTIES m_slots[4];
    EAXREVERBPROPERTIES m_reverb[4];
    EAXCONTEXTPROPERTIES m_context;
    EAXREVERBPROPERTIES m_committed;
    u32 m_dirty;
    int m_lastError;
    float m_decayTimeBase[4], m_decayTimeSize[4], m_reflectionsBase[4], m_reflectionsSize[4];
    float m_reflectionsDelayBase[4], m_reflectionsDelaySize[4], m_reverbBase[4], m_reverbSize[4];
    float m_reverbDelayBase[4], m_reverbDelaySize[4], m_echoTimeBase[4], m_echoTimeSize[4];
    float m_modulationTimeBase[4], m_modulationTimeSize[4];
    void Reset()
    {
        std::memset(this, 0, sizeof(*this));
        m_primarySlot = 1;
        for (int i = 0; i < 4; ++i) { m_reverb[i].flEnvironmentSize = 7.5f + i; m_slots[i].guidLoadEffect = EAX_REVERB_EFFECT; }
        m_slots[1].ulFlags = 1;                    // slot 1: primary, environment flag set
        m_slots[2].ulFlags = 1;                    // slot 2: environment flag but not primary
    }
    int ClampFloatProperty(float* pValue, float minimum, float maximum, int clamp)
    {
        if (IsNaN(*pValue)) return -3;
        if (*pValue < minimum) { if (clamp || *pValue >= minimum * 0.99999988f) { *pValue = minimum; return 0; } return -3; }
        if (*pValue > maximum) { if (clamp || *pValue * 0.99999988f <= maximum) *pValue = maximum; else return -3; }
        return 0;
    }
    int ClampLongProperty(int* pValue, int minimum, int maximum, bool clamp)
    {
        if (*pValue < minimum) { if (!clamp) return -3; *pValue = minimum; }
        else if (*pValue > maximum) { if (!clamp) return -3; *pValue = maximum; }
        return 0;
    }
    int SetDecayTimeScale(u32 slot, int enable)
    {
        u32& flags = m_reverb[slot].ulFlags;
        if (enable != (int)((flags >> 0) & 1))
        {
            if (enable)
            {
                flags |= 0x1;
                if (m_primarySlot == (int)slot && (m_slots[slot].ulFlags & 1)) { m_committed.ulFlags |= 0x1; m_dirty |= 0x800000; }
            }
            else
            {
                flags &= ~0x1u;
                if (m_primarySlot == (int)slot && (m_slots[slot].ulFlags & 1)) { m_committed.ulFlags &= ~0x1u; m_dirty |= 0x800000; }
            }
        }
        return 0;
    }
};

int main()
{
    static CEaxContext c;
    c.Reset();
    c.m_reverb[1].ulFlags = 0x100; c.m_committed.ulFlags = 0x200;
    if (c.SetDecayTimeScale(1, 1) != 0 || c.m_reverb[1].ulFlags != (0x100u | 0x1) || c.m_committed.ulFlags != (0x200u | 0x1) || c.m_dirty != 0x800000) { std::printf("00c63f90_TEST FAIL\n"); return 1; }
    c.m_dirty = 0;
    if (c.SetDecayTimeScale(1, 1) != 0 || c.m_dirty != 0) { std::printf("00c63f90_TEST FAIL\n"); return 1; }             // unchanged
    if (c.SetDecayTimeScale(1, 0) != 0 || c.m_reverb[1].ulFlags != 0x100u || c.m_committed.ulFlags != 0x200u || c.m_dirty != 0x800000) { std::printf("00c63f90_TEST FAIL\n"); return 1; }
    c.m_dirty = 0;
    if (c.SetDecayTimeScale(2, 1) != 0 || c.m_reverb[2].ulFlags != 0x1u || c.m_dirty != 0) { std::printf("00c63f90_TEST FAIL\n"); return 1; }   // not the primary slot
    std::printf("00c63f90_TEST PASS\n");
    return 0;
}