// Test for 0x00C674B0 CEaxContext::SetSlotLoadEffect: reserved slots, the reverb defaults and primary-slot commit with the
// volume folded and clamped, the null-GUID unload, and an unknown effect.
#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memcmp)
#endif
struct SGuid {                                     // GUID, 16 bytes
    unsigned long  Data1;
    unsigned short Data2, Data3;
    unsigned char  Data4[8];
};
struct EAXVECTOR {
    float x, y, z;
};
struct EAXFXSLOTPROPERTIES {                       // EAX 4.0, 0x1c bytes
    SGuid         guidLoadEffect;                  // +0x00
    long          lVolume;                         // +0x10 -10000..0 mB
    long          lLock;                           // +0x14
    unsigned long ulFlags;                         // +0x18 bit 0: EAXFXSLOTFLAGS_ENVIRONMENT
};
struct EAXREVERBPROPERTIES {                       // EAX 4.0, 0x70 bytes
    unsigned long ulEnvironment;                   // +0x00
    float         flEnvironmentSize;               // +0x04
    float         flEnvironmentDiffusion;          // +0x08
    long          lRoom;                           // +0x0c
    long          lRoomHF;                         // +0x10
    long          lRoomLF;                         // +0x14
    float         flDecayTime;                     // +0x18
    float         flDecayHFRatio;                  // +0x1c
    float         flDecayLFRatio;                  // +0x20
    long          lReflections;                    // +0x24
    float         flReflectionsDelay;              // +0x28
    EAXVECTOR     vReflectionsPan;                 // +0x2c
    long          lReverb;                         // +0x38
    float         flReverbDelay;                   // +0x3c
    EAXVECTOR     vReverbPan;                      // +0x40
    float         flEchoTime;                      // +0x4c
    float         flEchoDepth;                     // +0x50
    float         flModulationTime;                // +0x54
    float         flModulationDepth;               // +0x58
    float         flAirAbsorptionHF;               // +0x5c
    float         flHFReference;                   // +0x60
    float         flLFReference;                   // +0x64
    float         flRoomRolloffFactor;             // +0x68
    unsigned long ulFlags;                         // +0x6c EAXREVERBFLAGS_* (bits 0..7)
};
struct EAXCONTEXTPROPERTIES {                      // EAX 4.0, 0x1c bytes
    SGuid         guidPrimaryFXSlotID;             // +0x00
    float         flDistanceFactor;                // +0x10
    float         flAirAbsorptionHF;               // +0x14
    float         flHFReference;                   // +0x18
};

class CEaxContext {                                // vtable 0x012B9340 (the vptr is not modelled here)
public:
    unsigned char        _vptr[4];                 // +0x000
    long                 m_primarySlot;            // +0x004 index of the primary FX slot, -1 = none
    unsigned char        _pad008[0x7c];            // +0x008
    EAXFXSLOTPROPERTIES  m_slots[4];               // +0x084
    EAXREVERBPROPERTIES  m_reverb[4];              // +0x0f4 one reverb record per FX slot
    EAXCONTEXTPROPERTIES m_context;                // +0x2b4
    unsigned char        _pad2d0[0x70];            // +0x2d0
    EAXREVERBPROPERTIES  m_committed;              // +0x340 the reverb the device renders
    unsigned char        _pad3b0[0xc];             // +0x3b0
    unsigned long        m_dirty;                  // +0x3bc committed-property dirty mask
    unsigned char        _pad3c0[0x14];            // +0x3c0
    long                 m_lastError;              // +0x3d4 -1 = reserved slot, -4 = unknown effect

    long SetSlotLoadEffect(unsigned long slot, SGuid effect);
};
const SGuid EAX_REVERB_EFFECT = { 0x0CF95C8F, 0xA3CC, 0x4849, { 0xB0, 0xB6, 0x83, 0x2E, 0xCC, 0x18, 0x22, 0xDF } };
const SGuid EAX_NULL_GUID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
inline bool operator==(const SGuid& a, const SGuid& b) { return !memcmp(&a, &b, sizeof(SGuid)); }

long CEaxContext::SetSlotLoadEffect(unsigned long slot, SGuid effect)
{
    if (slot == 0 || slot == 1) {
        m_lastError = -1;
        return -3;
    }
    if (effect == EAX_REVERB_EFFECT) {
        m_reverb[slot].ulEnvironment = 0;
        m_reverb[slot].flEnvironmentSize = 7.5f;
        m_reverb[slot].flEnvironmentDiffusion = 1.0f;
        m_reverb[slot].lRoom = -1000;
        m_reverb[slot].lRoomHF = -100;
        m_reverb[slot].lRoomLF = 0;
        m_reverb[slot].flDecayTime = 1.49f;
        m_reverb[slot].flDecayHFRatio = 0.83f;
        m_reverb[slot].flDecayLFRatio = 1.0f;
        m_reverb[slot].lReflections = -2602;
        m_reverb[slot].flReflectionsDelay = 0.007f;
        m_reverb[slot].vReflectionsPan.x = 0.0f;
        m_reverb[slot].vReflectionsPan.y = 0.0f;
        m_reverb[slot].vReflectionsPan.z = 0.0f;
        m_reverb[slot].lReverb = 200;
        m_reverb[slot].flReverbDelay = 0.011f;
        m_reverb[slot].vReverbPan.x = 0.0f;
        m_reverb[slot].vReverbPan.y = 0.0f;
        m_reverb[slot].vReverbPan.z = 0.0f;
        m_reverb[slot].flEchoTime = 0.25f;
        m_reverb[slot].flEchoDepth = 0.0f;
        m_reverb[slot].flModulationTime = 0.25f;
        m_reverb[slot].flModulationDepth = 0.0f;
        m_reverb[slot].flAirAbsorptionHF = -5.0f;
        m_reverb[slot].flHFReference = 5000.0f;
        m_reverb[slot].flLFReference = 250.0f;
        m_reverb[slot].flRoomRolloffFactor = 0.0f;
        m_reverb[slot].ulFlags = 0x3f;
        m_slots[slot].guidLoadEffect = EAX_REVERB_EFFECT;
        if (m_primarySlot == (long)slot && (m_slots[slot].ulFlags & 1)) {
            m_committed = m_reverb[slot];
            m_committed.lRoom += m_slots[slot].lVolume;
            if (m_committed.lRoom < -10000)
                m_committed.lRoom = -10000;
            m_dirty |= 0x1000000;
        }
    } else if (effect == EAX_NULL_GUID) {
        m_slots[slot].guidLoadEffect = EAX_NULL_GUID;
        if (m_primarySlot == (long)slot) {
            m_committed.flAirAbsorptionHF = m_context.flAirAbsorptionHF;
            m_committed.lRoom = -10000;
            m_committed.flHFReference = m_context.flHFReference;
            m_dirty |= 0x180008;
        }
    } else {
        m_lastError = -4;
        return -3;
    }
    return 0;
}

static int Near(float a, float b) { float d = a - b; return d < 1e-6f && d > -1e-6f; }
int main(void)
{
    int ok = 1;
    static CEaxContext ctx;
    memset(&ctx, 0, sizeof(ctx));
    SGuid other = { 1, 2, 3, { 4, 5, 6, 7, 8, 9, 10, 11 } };
    if (ctx.SetSlotLoadEffect(0, EAX_REVERB_EFFECT) != -3 || ctx.m_lastError != -1) ok = 0;
    ctx.m_lastError = 0;
    if (ctx.SetSlotLoadEffect(1, EAX_NULL_GUID) != -3 || ctx.m_lastError != -1) ok = 0;
    ctx.m_lastError = 0;
    if (ctx.SetSlotLoadEffect(2, other) != -3 || ctx.m_lastError != -4) ok = 0;
    ctx.m_lastError = 0;
    /* slot 2 is not the primary slot: defaults land in m_reverb[2] only */
    ctx.m_primarySlot = 3;
    if (ctx.SetSlotLoadEffect(2, EAX_REVERB_EFFECT) != 0 || ctx.m_lastError != 0) ok = 0;
    EAXREVERBPROPERTIES& r = ctx.m_reverb[2];
    if (r.ulEnvironment != 0 || !Near(r.flEnvironmentSize, 7.5f) || !Near(r.flEnvironmentDiffusion, 1.0f) || r.lRoom != -1000
        || r.lRoomHF != -100 || r.lRoomLF != 0 || !Near(r.flDecayTime, 1.49f) || !Near(r.flDecayHFRatio, 0.83f)
        || !Near(r.flDecayLFRatio, 1.0f) || r.lReflections != -2602 || !Near(r.flReflectionsDelay, 0.007f)
        || r.lReverb != 200 || !Near(r.flReverbDelay, 0.011f) || !Near(r.flEchoTime, 0.25f) || !Near(r.flModulationTime, 0.25f)
        || !Near(r.flAirAbsorptionHF, -5.0f) || !Near(r.flHFReference, 5000.0f) || !Near(r.flLFReference, 250.0f)
        || r.ulFlags != 0x3f || r.flEchoDepth != 0.0f || r.flModulationDepth != 0.0f || r.flRoomRolloffFactor != 0.0f) ok = 0;
    if (!(ctx.m_slots[2].guidLoadEffect == EAX_REVERB_EFFECT) || ctx.m_dirty != 0 || ctx.m_committed.lRoom != 0) ok = 0;
    /* primary slot 3 with the environment flag: commit, volume folded and clamped */
    ctx.m_slots[3].ulFlags = 1; ctx.m_slots[3].lVolume = -9500;
    if (ctx.SetSlotLoadEffect(3, EAX_REVERB_EFFECT) != 0) ok = 0;
    if (ctx.m_committed.lRoom != -10000 || ctx.m_dirty != 0x1000000 || !Near(ctx.m_committed.flDecayTime, 1.49f)
        || ctx.m_committed.ulFlags != 0x3f) ok = 0;
    ctx.m_slots[3].lVolume = -500; ctx.m_dirty = 0;
    if (ctx.SetSlotLoadEffect(3, EAX_REVERB_EFFECT) != 0 || ctx.m_committed.lRoom != -1500 || ctx.m_dirty != 0x1000000) ok = 0;
    /* primary slot without the environment flag: no commit */
    ctx.m_slots[3].ulFlags = 0; ctx.m_dirty = 0; ctx.m_committed.lRoom = 77;
    if (ctx.SetSlotLoadEffect(3, EAX_REVERB_EFFECT) != 0 || ctx.m_committed.lRoom != 77 || ctx.m_dirty != 0) ok = 0;
    /* unload the primary slot */
    ctx.m_context.flAirAbsorptionHF = -2.5f; ctx.m_context.flHFReference = 4000.0f;
    if (ctx.SetSlotLoadEffect(3, EAX_NULL_GUID) != 0) ok = 0;
    if (!(ctx.m_slots[3].guidLoadEffect == EAX_NULL_GUID) || ctx.m_committed.lRoom != -10000 || ctx.m_dirty != 0x180008
        || !Near(ctx.m_committed.flAirAbsorptionHF, -2.5f) || !Near(ctx.m_committed.flHFReference, 4000.0f)) ok = 0;
    /* unload a non-primary slot: only the GUID changes */
    ctx.m_dirty = 0; ctx.m_committed.lRoom = 5;
    if (ctx.SetSlotLoadEffect(2, EAX_NULL_GUID) != 0 || ctx.m_dirty != 0 || ctx.m_committed.lRoom != 5
        || !(ctx.m_slots[2].guidLoadEffect == EAX_NULL_GUID)) ok = 0;
    printf(ok ? "00c674b0_TEST PASS\n" : "00c674b0_TEST FAIL\n");
    return 0;
}