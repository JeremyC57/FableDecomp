// CEaxContext::SetReflectionsScale(unsigned long slot, long enable) -- set or clear EAX 4.0 reverb flag EAXREVERBFLAGS_REFLECTIONSSCALE (bit 1) of an FX slot when it changes; for the primary slot with the environment flag the committed flags follow (dirty bit 0x800000); returns 0
// Leaf: SetReflectionsScale
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C64020, 137 bytes (`__fastcall`, `ret 8`).
// Identity: the manifest label is propagated (a game-side name). The object is the audio wrapper's software EAX 4.0
//   context (vtable 0x012B9340, destructor 0x00C60930): four FX slots (EAXFXSLOTPROPERTIES, 0x1c bytes, at +0x84), one
//   EAXREVERBPROPERTIES record (0x70 bytes) per slot at +0xF4, the EAXCONTEXTPROPERTIES record at +0x2B4, the committed
//   reverb the device renders at +0x340, its dirty mask at +0x3BC and the last error at +0x3D4 (-2 = invalid
//   parameter; the setters return -3). Ranges and offsets are the EAX 4.0 SDK's (EAXREVERB_MIN/MAX..., EAXCONTEXT_...);
//   the slot GUIDs at 0x012B9468.. are EAXPROPERTYID_EAX40_FXSlot0..3 and 0x012B94B8 is EAX_REVERB_EFFECT. No PDB
//   or RTTI covers the library, so the class and method names are descriptive. Local model (no engine header).
// Shape: `unsigned long& flags` (lea esi) and the old bit `((flags >> 1) & 1)` compared with the long argument; the two
//   arms each carry the primary-slot test and VC7.1 merges their committed-flags store and dirty-mask tail.
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
    long SetReflectionsScale(unsigned long slot, long enable);
};

long CEaxContext::SetReflectionsScale(unsigned long slot, long enable)
{
    unsigned long& flags = m_reverb[slot].ulFlags;
    if (enable != (long)((flags >> 1) & 1))
    {
        if (enable)
        {
            flags |= 0x2;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
            {
                m_committed.ulFlags |= 0x2;
                m_dirty |= 0x800000;
            }
        }
        else
        {
            flags &= ~0x2;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
            {
                m_committed.ulFlags &= ~0x2;
                m_dirty |= 0x800000;
            }
        }
    }
    return 0;
}