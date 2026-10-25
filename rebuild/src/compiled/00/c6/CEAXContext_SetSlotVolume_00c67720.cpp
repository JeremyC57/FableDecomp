// CEAXContext::SetSlotVolume(long slot, long volume) -- EAX 4.0 FX slot lVolume (-10000..0 mB, else last error -2 / result -3); on a change store it and, for the current slot loaded with the reverb effect, recommit the room level (slot reverb lRoom + volume, floored at -10000) with dirty bit 8; returns 0
// Leaf: ?SetSlotVolume@CEAXContext
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C67720, 146 bytes (+ int3 padding), `ret 8`.
// Callees: none (the GUID compare is the `repe cmpsd` memcmp intrinsic); data: EAX_REVERB_EFFECT GUID @ 0x012B94B8.
// Callers: 0x00C68A12 and 0x00C694BC (the slot property dispatchers).
// Shape: the `||` range test puts the error block at the end; `volume + lRoom` (volume first: `add ebx,edx`) is stored
//   and re-tested from the register; memcmp(...) == 0 inside a nested if after the current-slot test.
// Module: libpk's EAX 4.0 property layer (0x00C63xxx..0x00C69xxx; no PDB/RTTI, no trace strings). The object is the EAX
//   context the landed 0x00C630D0 (SetSlotEnabled, filed as "CLight") models: current FX slot +0x04, four 0x1c-byte
//   EAXFXSLOTPROPERTIES from +0x84 (guidLoadEffect, lVolume +0x10, lLock, ulFlags +0x18), four 0x70-byte
//   EAXREVERBPROPERTIES (one per slot) from +0xf4, the committed reverb record +0x340, dirty mask +0x3bc, last error +0x3d4.
// Layout: local model (no PDB class); member names follow the EAX 4.0 SDK structures.
#include <string.h>
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
extern const SGuid g_EAXReverbEffectGuid;        // 0x012B94B8
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
    long SetSlotVolume(long slot, long volume);
};
long CEAXContext::SetSlotVolume(long slot, long volume)
{
    if (volume < -10000 || volume > 0) {
        m_lastError = -2;
        return -3;
    }
    if (volume != m_slots[slot].lVolume) {
        m_slots[slot].lVolume = volume;
        if (m_currentSlot == slot) {
            if (memcmp(&m_slots[slot].guidLoadEffect, &g_EAXReverbEffectGuid, sizeof(SGuid)) == 0) {
                m_committed.lRoom = volume + m_slotReverb[slot].lRoom;
                if (m_committed.lRoom < -10000)
                    m_committed.lRoom = -10000;
                m_dirty |= 8;
            }
        }
    }
    return 0;
}