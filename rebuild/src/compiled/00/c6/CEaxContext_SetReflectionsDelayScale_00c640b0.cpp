// CEaxContext::SetReflectionsDelayScale(unsigned long slot, long enable) -- set or clear EAX 4.0 reverb flag EAXREVERBFLAGS_REFLECTIONSDELAYSCALE (bit 2) of an FX slot when it changes; for the primary slot with the environment flag the committed flags follow (dirty bit 0x800000); returns 0
// Leaf: ?SetReflectionsDelayScale@CEaxContext@@QAEJKJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C640B0, 138 bytes, thiscall, `ret 8`.
// Identity (probable): the landed CEaxContext::SetReverbFlags 0x00C67A10 applies the eight EAXREVERBFLAGS bits through eight
//   same-TU setters in bit order (0x00C63F90..0x00C64380); this one, called at 0x00C67A75, flips bit 2 (0x4) -- the EAX 4.0 SDK's
//   EAXREVERBFLAGS_REFLECTIONSDELAYSCALE -- of m_reverb[slot].ulFlags (+0x160 + 0x70 x slot). Class and method names are sweep1's descriptive libpk names
//   (no PDB/RTTI); the second caller is 0x00C68A50.
// Shape (family K063, representative 0x00C640B0): the landed SetReflectionsScale 0x00C64020 (bit 1) with bit 2.
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
    long          lVolume;                         // +0x10
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
    unsigned long ulFlags;                         // +0x6c
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
    float                m_decayTimeBase[4];       // +0x3d8 recorded values and the environment size they
    float                m_decayTimeSize[4];       // +0x3e8   were set at (environment-size scaling)
    float                m_reflectionsBase[4];     // +0x3f8
    float                m_reflectionsSize[4];     // +0x408
    float                m_reflectionsDelayBase[4];// +0x418
    float                m_reflectionsDelaySize[4];// +0x428
    float                m_reverbBase[4];          // +0x438
    float                m_reverbSize[4];          // +0x448
    float                m_reverbDelayBase[4];     // +0x458
    float                m_reverbDelaySize[4];     // +0x468
    float                m_echoTimeBase[4];        // +0x478
    float                m_echoTimeSize[4];        // +0x488
    float                m_modulationTimeBase[4];  // +0x498
    float                m_modulationTimeSize[4];  // +0x4a8

    long ClampFloatProperty(float* pValue, float minimum, float maximum, int clamp);   // 0x00C60450
    long SetReflectionsDelayScale(unsigned long slot, long enable);
};

long CEaxContext::SetReflectionsDelayScale(unsigned long slot, long enable)
{
    unsigned long& flags = m_reverb[slot].ulFlags;
    if (enable != (long)((flags >> 2) & 1))
    {
        if (enable)
        {
            flags |= 0x4;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
            {
                m_committed.ulFlags |= 0x4;
                m_dirty |= 0x800000;
            }
        }
        else
        {
            flags &= ~0x4;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
            {
                m_committed.ulFlags &= ~0x4;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}