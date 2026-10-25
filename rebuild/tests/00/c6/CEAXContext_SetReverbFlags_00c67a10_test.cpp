#include <cstdio>
#include <cstring>
struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct SEAXFXSlotProperties {                     // EAX 4.0 EAXFXSLOTPROPERTIES, 0x1c bytes
    SGuid         guidLoadEffect;                 // +0x00
    long          lVolume;                        // +0x10 -10000..0 mB
    long          lLock;                          // +0x14
    unsigned long ulFlags;                        // +0x18 bit 0 EAXFXSLOTFLAGS_ENVIRONMENT
};
struct SEAXVector { float x, y, z; };
struct SEAXReverbProperties {                     // EAX 3.0/4.0 EAXREVERBPROPERTIES, 0x70 bytes
    unsigned long ulEnvironment;                  // +0x00
    float         flEnvironmentSize;              // +0x04
    float         flEnvironmentDiffusion;         // +0x08
    long          lRoom;                          // +0x0c
    long          lRoomHF;                        // +0x10
    long          lRoomLF;                        // +0x14
    float         flDecayTime;                    // +0x18
    float         flDecayHFRatio;                 // +0x1c
    float         flDecayLFRatio;                 // +0x20
    long          lReflections;                   // +0x24
    float         flReflectionsDelay;             // +0x28
    SEAXVector    vReflectionsPan;                // +0x2c
    long          lReverb;                        // +0x38
    float         flReverbDelay;                  // +0x3c
    SEAXVector    vReverbPan;                     // +0x40
    float         flEchoTime;                     // +0x4c
    float         flEchoDepth;                    // +0x50
    float         flModulationTime;               // +0x54
    float         flModulationDepth;              // +0x58
    float         flAirAbsorptionHF;              // +0x5c
    float         flHFReference;                  // +0x60
    float         flLFReference;                  // +0x64
    float         flRoomRolloffFactor;            // +0x68
    unsigned long ulFlags;                        // +0x6c EAXREVERBFLAGS_* (bits 0..7)
};
struct CEAXContext {
    unsigned char        _pad000[0x4];
    long                 m_currentSlot;           // +0x04
    unsigned char        _pad008[0x84 - 0x8];
    SEAXFXSlotProperties m_slots[4];              // +0x84
    SEAXReverbProperties m_slotReverb[4];         // +0xf4
    unsigned char        _pad2b4[0x340 - 0x2b4];
    SEAXReverbProperties m_committed;             // +0x340
    unsigned char        _pad3b0[0x3bc - 0x3b0];
    unsigned long        m_dirty;                 // +0x3bc
    unsigned char        _pad3c0[0x3d4 - 0x3c0];
    long                 m_lastError;             // +0x3d4
    long SetDecayTimeScaleFlag(long slot, long on);            // 0x00C63F90
    long SetReflectionsScaleFlag(long slot, long on);          // 0x00C64020
    long SetReflectionsDelayScaleFlag(long slot, long on);     // 0x00C640B0
    long SetReverbScaleFlag(long slot, long on);               // 0x00C64140
    long SetReverbDelayScaleFlag(long slot, long on);          // 0x00C641D0
    long SetDecayHFLimitFlag(long slot, long on);              // 0x00C64260
    long SetEchoTimeScaleFlag(long slot, long on);             // 0x00C642F0
    long SetModulationTimeScaleFlag(long slot, long on);       // 0x00C64380
    long SetReverbFlags(long slot, unsigned long flags);
};

long CEAXContext::SetDecayTimeScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags) & 1)) {
        if (on) {
            *flags |= 0x1UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x1UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x1UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x1UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetReflectionsScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 1) & 1)) {
        if (on) {
            *flags |= 0x2UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x2UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x2UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x2UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetReflectionsDelayScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 2) & 1)) {
        if (on) {
            *flags |= 0x4UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x4UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x4UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x4UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetReverbScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 3) & 1)) {
        if (on) {
            *flags |= 0x8UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x8UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x8UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x8UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetReverbDelayScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 4) & 1)) {
        if (on) {
            *flags |= 0x10UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x10UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x10UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x10UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetDecayHFLimitFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 5) & 1)) {
        if (on) {
            *flags |= 0x20UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x20UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x20UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x20UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetEchoTimeScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 6) & 1)) {
        if (on) {
            *flags |= 0x40UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x40UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x40UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x40UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}

long CEAXContext::SetModulationTimeScaleFlag(long slot, long on)
{
    unsigned long* flags = &m_slotReverb[slot].ulFlags;
    if (on != (long)((*flags >> 7) & 1)) {
        if (on) {
            *flags |= 0x80UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags |= 0x80UL;
                m_dirty |= 0x800000;
            }
        } else {
            *flags &= ~0x80UL;
            if (m_currentSlot == slot && (m_slots[slot].ulFlags & 1)) {
                m_committed.ulFlags &= ~0x80UL;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}


long CEAXContext::SetReverbFlags(long slot, unsigned long flags)
{
    long result = 0;
    if (flags & ~0xffUL) {
        m_lastError = -2;
        return -3;
    }
    if (m_slotReverb[slot].ulFlags != flags) {
        if ((result = SetDecayTimeScaleFlag(slot, flags & 1)) < 0) return result;
        if ((result = SetReflectionsScaleFlag(slot, (flags >> 1) & 1)) < 0) return result;
        if ((result = SetReflectionsDelayScaleFlag(slot, (flags >> 2) & 1)) < 0) return result;
        if ((result = SetReverbScaleFlag(slot, (flags >> 3) & 1)) < 0) return result;
        if ((result = SetReverbDelayScaleFlag(slot, (flags >> 4) & 1)) < 0) return result;
        if ((result = SetDecayHFLimitFlag(slot, (flags >> 5) & 1)) < 0) return result;
        if ((result = SetEchoTimeScaleFlag(slot, (flags >> 6) & 1)) < 0) return result;
        result = SetModulationTimeScaleFlag(slot, (flags >> 7) & 1);
    }
    return result;
}
static int fail() { std::printf("00c67a10_TEST FAIL\n"); return 1; }
int main()
{
    static CEAXContext c;
    std::memset(&c, 0, sizeof(c));
    if (c.SetReverbFlags(0, 0x100) != -3 || c.m_lastError != -2) return fail();
    c.m_lastError = 0;
    // slot 2 is current and an active environment slot: both records follow
    c.m_currentSlot = 2;
    c.m_slots[2].ulFlags = 1;
    if (c.SetReverbFlags(2, 0xA5) != 0) return fail();
    if (c.m_slotReverb[2].ulFlags != 0xA5 || c.m_committed.ulFlags != 0xA5 || c.m_dirty != 0x800000) return fail();
    if (c.SetReverbFlags(2, 0x5A) != 0) return fail();
    if (c.m_slotReverb[2].ulFlags != 0x5A || c.m_committed.ulFlags != 0x5A) return fail();
    // another slot: only its own record changes
    c.m_dirty = 0;
    if (c.SetReverbFlags(1, 0xFF) != 0 || c.m_slotReverb[1].ulFlags != 0xFF || c.m_committed.ulFlags != 0x5A || c.m_dirty != 0) return fail();
    // unchanged: nothing happens
    if (c.SetReverbFlags(1, 0xFF) != 0 || c.m_dirty != 0) return fail();
    std::printf("00c67a10_TEST PASS\n");
    return 0;
}