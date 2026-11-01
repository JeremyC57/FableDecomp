// CEaxContext::SetEnvironmentSize(unsigned long slot, float value) -- EAX 4.0 reverb EnvironmentSize of an FX slot: range 1.0..100.0 (else last error -2, result -3); an unchanged value is a no-op; a changed one is stored (and committed with dirty bit 2 for the primary slot with the environment flag) and then rescales the slot's size-scaled properties per its EAXREVERBFLAGS: DecayTime, Reflections (when both reflections flags are set, by -2000 log10 of the size ratio), ReflectionsDelay, Reverb (inline: by -2000 log10 of the ratio, 1.5x unless DecayTimeScale, clamped to -10000..2000, committed with dirty bit 0x1000), ReverbDelay, EchoTime and ModulationTime from the recorded bases; returns 0
// Leaf: ?SetEnvironmentSize@CEaxContext@@QAEJKM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C677C0, 582 bytes, thiscall, `ret 8`.
// Identity (descriptive; libpk has no PDB or RTTI): the object is the landed CEaxContext (vtable 0x012B9340; FX slots at +0x84,
//   EAXREVERBPROPERTIES records at +0xF4, committed reverb +0x340, dirty mask +0x3BC, last error +0x3D4, the recorded base/size
//   pairs +0x3D8..); the range 1.0..100.0 is EAXREVERB_MINENVIRONMENTSIZE..MAXENVIRONMENTSIZE, the field +4 of the record is
//   flEnvironmentSize, and the flag bits tested are the EAX 4.0 EAXREVERBFLAGS (DECAYTIMESCALE 1, REFLECTIONSSCALE 2,
//   REFLECTIONSDELAYSCALE 4, REVERBSCALE 8, REVERBDELAYSCALE 0x10, ECHOTIMESCALE 0x40, MODULATIONTIMESCALE 0x80) in the SDK's
//   environment-size scaling rules. The callers 0x00C68CE1 / 0x00C68DA7 / 0x00C6908E are the context's EAX property dispatchers
//   (0x00C68A50 / 0x00C68F90). The catalog label "HeroAttribute_SetValueWithValidation" is propagated and wrong.
// Callees: CEaxContext::SetDecayTime 0x00C633C0, SetReflections 0x00C635C0, SetReflectionsDelay 0x00C63690, SetReverbDelay
//   0x00C638E0, SetEchoTime 0x00C63A60, SetModulationTime 0x00C63BC0 (all (slot, value, record = false, clamp = true), the landed
//   models), GainToMillibels 0x00C62FB0 (`__cdecl`, 2000 log10), __ftol2 0x00BFEA70 for the (long) casts. Constants 1.0f 0x0122DED8,
//   100.0f 0x01230A08.
// Shape: the range check inline (`value < 1.0f || value > 100.0f`, no ClampFloatProperty); `float& stored = m_reverb[slot].
//   flEnvironmentSize; if (stored == value) return 0;` (fucompp, `test ah,44h ; jnp`); the flags are re-read from the record
//   through a reference before each test (the calls may change them); the reverb scale `float scale = 1.0f; if (!(flags & 1))
//   scale = 1.5f;` lives in the dead `slot` home; the clamp is `if (v < -10000) v = -10000; else if (v > 2000) v = 2000;`.
float __cdecl GainToMillibels(float gain);         // 0x00C62FB0
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

    long SetDecayTime(unsigned long slot, float value, bool record, bool clamp);          // 0x00C633C0
    long SetReflections(unsigned long slot, long value, bool record, bool clamp);         // 0x00C635C0
    long SetReflectionsDelay(unsigned long slot, float value, bool record, bool clamp);   // 0x00C63690
    long SetReverbDelay(unsigned long slot, float value, bool record, bool clamp);        // 0x00C638E0
    long SetEchoTime(unsigned long slot, float value, bool record, bool clamp);           // 0x00C63A60
    long SetModulationTime(unsigned long slot, float value, bool record, bool clamp);     // 0x00C63BC0
    long SetEnvironmentSize(unsigned long slot, float value);                             // 0x00C677C0 (this row)
};

long CEaxContext::SetEnvironmentSize(unsigned long slot, float value)
{
    if (value < 1.0f || value > 100.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flEnvironmentSize;
    if (stored == value)
        return 0;
    stored = value;
    if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
    {
        m_committed.flEnvironmentSize = value;
        m_dirty |= 2;
    }
    unsigned long& flags = m_reverb[slot].ulFlags;
    if (flags & 1)
        SetDecayTime(slot, m_decayTimeBase[slot] / m_decayTimeSize[slot] * value, false, true);
    if ((flags & 2) && (flags & 4))
        SetReflections(slot, (long)(m_reflectionsBase[slot] - GainToMillibels(value / m_reflectionsSize[slot])), false, true);
    if (flags & 4)
        SetReflectionsDelay(slot, m_reflectionsDelayBase[slot] / m_reflectionsDelaySize[slot] * value, false, true);
    if (flags & 8)
    {
        float scale = 1.0f;
        if (!(flags & 1))
            scale = 1.5f;
        long reverb = (long)(m_reverbBase[slot] - GainToMillibels(value / m_reverbSize[slot]) * scale);
        if (reverb < -10000)
            reverb = -10000;
        else if (reverb > 2000)
            reverb = 2000;
        long& storedReverb = m_reverb[slot].lReverb;
        if (storedReverb != reverb)
        {
            storedReverb = reverb;
            if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
            {
                m_committed.lReverb = reverb;
                m_dirty |= 0x1000;
            }
        }
    }
    if (flags & 0x10)
        SetReverbDelay(slot, m_reverbDelayBase[slot] / m_reverbDelaySize[slot] * value, false, true);
    if (flags & 0x40)
        SetEchoTime(slot, m_echoTimeBase[slot] / m_echoTimeSize[slot] * value, false, true);
    if (flags & 0x80)
        SetModulationTime(slot, m_modulationTimeBase[slot] / m_modulationTimeSize[slot] * value, false, true);
    return 0;
}