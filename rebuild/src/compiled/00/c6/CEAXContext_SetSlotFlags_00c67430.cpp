// CEAXContext::SetSlotFlags(long slot, unsigned long flags) -- EAX 4.0 FX slot ulFlags: only EAXFXSLOTFLAGS_ENVIRONMENT (bit 0) is accepted (else -3, no error store); on a change hand (flags & 1) == 1 to SetSlotEnabled and return its result, else 0
// Leaf: ?SetSlotFlags@CEAXContext
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C67430, 64 bytes, `ret 8`.
// Callees: CEAXContext::SetSlotEnabled 0x00C630D0 (landed; flips bit 0 of the slot flags and commits/silences the room).
// Callers: 0x00C68A22 and 0x00C69508 (the slot property dispatchers).
// Shape: `long result = 0` declared first gives retail's `xor eax,eax` ahead of the flag test (it stays live, so the
//   slot address goes to edi); the change test is `flags != member` (`cmp edx,[edi+ecx+9Ch]`); `(flags & 1) == 1`
//   is the 8-bit `and dl,1 ; dec dl ; neg dl ; sbb edx,edx ; inc edx` the landed callee also uses.
// Module: libpk's EAX 4.0 property layer (0x00C63xxx..0x00C69xxx; no PDB/RTTI, no trace strings). The object is the EAX
//   context the landed 0x00C630D0 (SetSlotEnabled, filed as "CLight") models: current FX slot +0x04, four 0x1c-byte
//   EAXFXSLOTPROPERTIES from +0x84 (guidLoadEffect, lVolume +0x10, lLock, ulFlags +0x18), four 0x70-byte
//   EAXREVERBPROPERTIES (one per slot) from +0xf4, the committed reverb record +0x340, dirty mask +0x3bc, last error +0x3d4.
// Layout: local model (no PDB class); member names follow the EAX 4.0 SDK structures.
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
    long SetSlotEnabled(long slot, long enabled); // 0x00C630D0
    long SetSlotFlags(long slot, unsigned long flags);
};
long CEAXContext::SetSlotFlags(long slot, unsigned long flags)
{
    long result = 0;
    if (flags & ~1UL)
        return -3;
    if (flags != m_slots[slot].ulFlags)
        result = SetSlotEnabled(slot, (flags & 1) == 1);
    return result;
}