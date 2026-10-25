// Behaviour test for 00c67a10: CEaxContext::SetReverbFlags on the EAX 4.0 context model
#include <cstdio>
#include <cstring>
#include <string.h>
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
const SGuid EAX_REVERB_EFFECT = { 0x0cf95c8f, 0xa3cc, 0x4849, { 0xb0, 0xb6, 0x83, 0x2e, 0xcc, 0x18, 0x22, 0xdf } };

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
    long                 m_lastError;              // +0x3d4 -2 = invalid parameter

    long SetDecayTimeScale(unsigned long slot, long enable);             // 0x00C63F90
    long SetReflectionsScale(unsigned long slot, long enable);           // 0x00C64020
    long SetReflectionsDelayScale(unsigned long slot, long enable);      // 0x00C640B0
    long SetReverbScale(unsigned long slot, long enable);                // 0x00C64140
    long SetReverbDelayScale(unsigned long slot, long enable);           // 0x00C641D0
    long SetDecayHFLimit(unsigned long slot, long enable);               // 0x00C64260
    long SetEchoTimeScale(unsigned long slot, long enable);              // 0x00C642F0
    long SetModulationTimeScale(unsigned long slot, long enable);        // 0x00C64380
    long SetReverbFlags(unsigned long slot, unsigned long flags);
};

long CEaxContext::SetDecayTimeScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags) & 1)) {
        if (enable) {
            *flags |= 0x1UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x1UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x1UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x1UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetReflectionsScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 1) & 1)) {
        if (enable) {
            *flags |= 0x2UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x2UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x2UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x2UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetReflectionsDelayScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 2) & 1)) {
        if (enable) {
            *flags |= 0x4UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x4UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x4UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x4UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetReverbScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 3) & 1)) {
        if (enable) {
            *flags |= 0x8UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x8UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x8UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x8UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetReverbDelayScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 4) & 1)) {
        if (enable) {
            *flags |= 0x10UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x10UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x10UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x10UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetDecayHFLimit(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 5) & 1)) {
        if (enable) {
            *flags |= 0x20UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x20UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x20UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x20UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetEchoTimeScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 6) & 1)) {
        if (enable) {
            *flags |= 0x40UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x40UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x40UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x40UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEaxContext::SetModulationTimeScale(unsigned long slot, long enable)
{
    unsigned long* flags = &m_reverb[slot].ulFlags;
    if (enable != (long)((*flags >> 7) & 1)) {
        if (enable) {
            *flags |= 0x80UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x80UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x80UL;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x80UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}


long CEaxContext::SetReverbFlags(unsigned long slot, unsigned long flags)
{
    long result = 0;
    if (flags & ~0xffUL) {
        m_lastError = -2;
        return -3;
    }
    if (m_reverb[slot].ulFlags != flags) {
        if ((result = SetDecayTimeScale(slot, flags & 1)) < 0) return result;
        if ((result = SetReflectionsScale(slot, (flags >> 1) & 1)) < 0) return result;
        if ((result = SetReflectionsDelayScale(slot, (flags >> 2) & 1)) < 0) return result;
        if ((result = SetReverbScale(slot, (flags >> 3) & 1)) < 0) return result;
        if ((result = SetReverbDelayScale(slot, (flags >> 4) & 1)) < 0) return result;
        if ((result = SetDecayHFLimit(slot, (flags >> 5) & 1)) < 0) return result;
        if ((result = SetEchoTimeScale(slot, (flags >> 6) & 1)) < 0) return result;
        result = SetModulationTimeScale(slot, (flags >> 7) & 1);
    }
    return result;
}

static CEaxContext g_c;
int main()
{
    CEaxContext& c = g_c; std::memset(&c, 0, sizeof c); c.m_primarySlot = 2; c.m_slots[2].ulFlags = 1; c.m_committed.ulFlags = 0;
    int ok = c.SetReverbFlags(2, 0x100) == -3 && c.m_lastError == -2;
    ok = ok && c.SetReverbFlags(2, 0xa5) == 0 && c.m_reverb[2].ulFlags == 0xa5 && c.m_committed.ulFlags == 0xa5 && c.m_dirty == 0x800000;
    c.m_dirty = 0; ok = ok && c.SetReverbFlags(2, 0xa5) == 0 && c.m_dirty == 0;
    ok = ok && c.SetReverbFlags(2, 0x0f) == 0 && c.m_reverb[2].ulFlags == 0x0f && c.m_committed.ulFlags == 0x0f;
    ok = ok && c.SetReverbFlags(1, 0x3) == 0 && c.m_reverb[1].ulFlags == 0x3 && c.m_committed.ulFlags == 0x0f;
    std::printf(ok ? "00c67a10_TEST PASS\n" : "00c67a10_TEST FAIL\n");
    return ok ? 0 : 1;
}
