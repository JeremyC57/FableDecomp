// CEaxContext::SetSlotHFReference(unsigned long slot, float value) -- EAX 4.0 reverb flHFReference of an FX slot (1000.0f..20000.0f, else last error -2 / result -3): store it in the slot's reverb record when it changed and, for the primary slot with EAXFXSLOTFLAGS_ENVIRONMENT, commit it (+0x3A0) with dirty bit 0x100000
// Leaf: ?SetSlotHFReference@CEaxContext@@QAEJKM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C63DB0, 146 bytes (160 with int3 padding), thiscall `ret 8`. Float pool:
//   0x01252370 = 1000.0f, 0x012B89C0 = 20000.0f.
// Identity (descriptive, descriptive name): the body stores into slot reverb record +0x154 (record base +0xF4, so field +0x60)
//   and commits to +0x3A0 (= 0x340 + 0x60), which is EAXREVERBPROPERTIES::flHFReference of the EAX 4.0 SDK; the range
//   1000.0f..20000.0f is the SDK's EAXREVERB_MIN/MAXHFREFERENCE, and the dirty bit 0x100000 is 1 << (field index 20), the
//   per-field scheme of the landed CEaxContext setters. No PDB or RTTI covers the library (libpk), so the method name is
//   descriptive: Set + the SDK field ("Slot" because EAXCONTEXTPROPERTIES has its own flHFReference, set by the landed context-level SetHFReference 0x00C646E0, as the landed SetSlotAirAbsorptionHF 0x00C63D20 does for flAirAbsorptionHF).
// Label: the manifest's "SetAttribute_Float_Validated" is propagated across family H033 (seven reverb float setters).
// Model: the landed CEaxContext::SetEnvironmentDiffusion 0x00C631E0 (same shape, field +0x08, dirty bit 4); local classes
//   (no engine header) -- Review at landing.
// Shape: the range test reads the argument twice (`< lo` = `test ah,5 ; jnp`, `> hi` = `test ah,41h ; je`) with the
//   failure block last; the change test is `stored != value` through a reference local.
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
    long                 m_lastError;              // +0x3d4 -2 = invalid parameter

    long SetSlotHFReference(unsigned long slot, float value);
};

long CEaxContext::SetSlotHFReference(unsigned long slot, float value)
{
    if (value < 1000.0f || value > 20000.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flHFReference;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flHFReference = value;
            m_dirty |= 0x100000;
        }
    }
    return 0;
}