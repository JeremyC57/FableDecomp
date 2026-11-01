// CEaxContext::SetEnvironment(unsigned long slot, unsigned long environment) -- libpk EAX 4.0: select a reverb environment preset for an FX slot: reject indices above 26 (last error -2, result -3), store the index, commit it to the hardware block when the slot is the primary enabled slot, and for a real preset push every preset parameter through the property setters in retail order, giving up at the first failing setter
// Leaf: ?SetEnvironment@CEaxContext@@QAEJKK@Z
// Identity (proven): the EAX 4.0 "ulEnvironment" setter of the libpk CEaxContext (vtable 0x012B9340) whose 31 sibling setters are
// landed byte-exact in rebuild/src/compiled/00/c6 (CEaxContext_Set*.cpp): same this layout (m_reverb[slot] at +0xf4 stride 0x70,
// m_slots[slot].ulFlags bit 0 at +0x84 stride 0x1c, m_committed +0x340, m_dirty +0x3bc, m_lastError +0x3d4), same -2/-3 error
// convention. environment > 26 (EAX_ENVIRONMENT_COUNT) is rejected; 26 = "no preset"; otherwise the 0x98-byte preset record
// g_EaxReverbPresets[environment] at 0x0139CA40 is pushed through every setter (record = false, clamp = true for the scaled ones), a
// negative setter result abandons the rest. The function always returns 0 after the parameter check (retail 0x00C68D62).
// No PDB/RTTI for libpk: descriptive names, consistent with the landed siblings. The manifest's "CDrawGuildSeal_SetLightProperty"
// and the CLight labels of 0x00C632E0/0x00C63350 are propagated stand-ins (NAME_FIXES).
// Same-TU: the landed sibling setters are compiled in this TU so VC7.1 knows (as retail's compiler did) which of them preserve ecx --
// retail reloads ecx only after the setters that clobber it.
#include <string.h>
extern "C" int __cdecl _isnan(double x);
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

    long ClampFloatProperty(float* pValue, float minimum, float maximum, int clamp);   // 0x00C60450
    long ClampLongProperty(long* pValue, long minimum, long maximum, bool clamp)
    {
        if (*pValue < minimum)
        {
            if (!clamp)
                return -3;
            *pValue = minimum;
        }
        else if (*pValue > maximum)
        {
            if (!clamp)
                return -3;
            *pValue = maximum;
        }
        return 0;
    }
    long SetRoom(unsigned long slot, long value);                                     // 0x00C63270
    long SetRoomHF(unsigned long slot, long value);                                   // 0x00C632E0 (landed as CLight::SetIntegerProperty2)
    long SetRoomLF(unsigned long slot, long value);                                   // 0x00C63350 (landed as CLight::SetIntegerProperty3)
    long SetDecayTime(unsigned long slot, float value, bool record, bool clamp);      // 0x00C633C0
    long SetDecayHFRatio(unsigned long slot, float value);                            // 0x00C63480
    long SetDecayLFRatio(unsigned long slot, float value);                            // 0x00C63520
    long SetReflections(unsigned long slot, long value, bool record, bool clamp);     // 0x00C635C0
    long SetReflectionsDelay(unsigned long slot, float value, bool record, bool clamp);   // 0x00C63690
    long SetReflectionsPan(unsigned long slot, EAXVECTOR value);                      // 0x00C63750
    long SetReverb(unsigned long slot, long value, bool record, bool clamp);          // 0x00C63810
    long SetReverbDelay(unsigned long slot, float value, bool record, bool clamp);    // 0x00C638E0
    long SetReverbPan(unsigned long slot, EAXVECTOR value);                           // 0x00C639A0
    long SetEnvironmentDiffusion(unsigned long slot, float value);                    // 0x00C631E0
    long SetEchoTime(unsigned long slot, float value, bool record, bool clamp);       // 0x00C63A60
    long SetEchoDepth(unsigned long slot, float value);                               // 0x00C63B20
    long SetModulationTime(unsigned long slot, float value, bool record, bool clamp); // 0x00C63BC0
    long SetModulationDepth(unsigned long slot, float value);                         // 0x00C63C80
    long SetSlotAirAbsorptionHF(unsigned long slot, float value);                     // 0x00C63D20
    long SetRoomRolloffFactor(unsigned long slot, float value);                       // 0x00C63EF0
    long SetSlotHFReference(unsigned long slot, float value);                         // 0x00C63DB0
    long SetLFReference(unsigned long slot, float value);                             // 0x00C63E50
    long SetReverbFlags(unsigned long slot, unsigned long flags);                     // 0x00C67A10
    long SetEnvironmentSize(unsigned long slot, float value);                         // 0x00C677C0
    long SetDecayTimeScale(unsigned long slot, long enable);                          // 0x00C63F90
    long SetReflectionsScale(unsigned long slot, long enable);                        // 0x00C64020
    long SetReflectionsDelayScale(unsigned long slot, long enable);                   // 0x00C640B0
    long SetReverbScale(unsigned long slot, long enable);                             // 0x00C64140
    long SetReverbDelayScale(unsigned long slot, long enable);                        // 0x00C641D0
    long SetDecayHFLimit(unsigned long slot, long enable);                            // 0x00C64260
    long SetEchoTimeScale(unsigned long slot, long enable);                           // 0x00C642F0
    long SetModulationTimeScale(unsigned long slot, long enable);                     // 0x00C64380
    long SetEnvironment(unsigned long slot, unsigned long environment);               // 0x00C68A50 (this row)
};

struct SEaxReverbPreset {                           // 0x98-byte entries of g_EaxReverbPresets (0x0139CA40)
    long          lRoom;                            // +0x00
    long          lRoomHF;                          // +0x04
    long          lRoomLF;                          // +0x08
    float         flRoomRolloffFactor;              // +0x0c
    float         flDecayTime;                      // +0x10
    float         flDecayHFRatio;                   // +0x14
    float         flDecayLFRatio;                   // +0x18
    long          lReflections;                     // +0x1c
    float         flReflectionsDelay;               // +0x20
    EAXVECTOR     vReflectionsPan;                  // +0x24
    long          lReverb;                          // +0x30
    float         flReverbDelay;                    // +0x34
    EAXVECTOR     vReverbPan;                       // +0x38
    unsigned char _pad44[4];                        // +0x44
    float         flEnvironmentSize;                // +0x48
    float         flEnvironmentDiffusion;           // +0x4c
    float         flAirAbsorptionHF;                // +0x50
    float         flEchoDepth;                      // +0x54
    float         flEchoTime;                       // +0x58
    float         flModulationDepth;                // +0x5c
    float         flModulationTime;                 // +0x60
    unsigned char _pad64[4];                        // +0x64
    long          lDecayTimeScale;                  // +0x68
    long          lReflectionsScale;                // +0x6c
    long          lReflectionsDelayScale;           // +0x70
    long          lReverbScale;                     // +0x74
    long          lReverbDelayScale;                // +0x78
    long          lEchoTimeScale;                   // +0x7c
    long          lModulationTimeScale;             // +0x80
    long          lDecayHFLimit;                    // +0x84
    unsigned char _pad88[8];                        // +0x88
    float         flHFReference;                    // +0x90
    float         flLFReference;                    // +0x94
};
extern const SEaxReverbPreset g_EaxReverbPresets[26];   // 0x0139CA40
// ---- 00c63270 (CEaxContext_SetRoom_00c63270.cpp) ----
__declspec(noinline) long CEaxContext::SetRoom(unsigned long slot, long value)
{
    if (value < -10000 || value > 0)
    {
        m_lastError = -2;
        return -3;
    }
    long& stored = m_reverb[slot].lRoom;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.lRoom = m_slots[slot].lVolume + value;
            m_dirty |= 8;
        }
    }
    return 0;
}
// ---- 00c633c0 (CEaxContext_SetDecayTime_00c633c0.cpp) ----
__declspec(noinline) long CEaxContext::SetDecayTime(unsigned long slot, float value, bool record, bool clamp)
{
    if (ClampFloatProperty(&value, 0.1f, 20.0f, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_decayTimeBase[slot] = value;
        m_decayTimeSize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    float& stored = m_reverb[slot].flDecayTime;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flDecayTime = value;
            m_dirty |= 0x40;
        }
    }
    return 0;
}
// ---- 00c63480 (CEaxContext_SetDecayHFRatio_00c63480.cpp) ----
__declspec(noinline) long CEaxContext::SetDecayHFRatio(unsigned long slot, float value)
{
    if (value < 0.1f || value > 2.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flDecayHFRatio;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flDecayHFRatio = value;
            m_dirty |= 0x80;
        }
    }
    return 0;
}
// ---- 00c63520 (CEaxContext_SetDecayLFRatio_00c63520.cpp) ----
__declspec(noinline) long CEaxContext::SetDecayLFRatio(unsigned long slot, float value)
{
    if (value < 0.1f || value > 2.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flDecayLFRatio;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flDecayLFRatio = value;
            m_dirty |= 0x100;
        }
    }
    return 0;
}
// ---- 00c635c0 (CEaxContext_SetReflections_00c635c0.cpp) ----
__declspec(noinline) long CEaxContext::SetReflections(unsigned long slot, long value, bool record, bool clamp)
{
    if (ClampLongProperty(&value, -10000, 1000, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_reflectionsBase[slot] = (float)value;
        m_reflectionsSize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    long& stored = m_reverb[slot].lReflections;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.lReflections = value;
            m_dirty |= 0x200;
        }
    }
    return 0;
}
// ---- 00c63690 (CEaxContext_SetReflectionsDelay_00c63690.cpp) ----
__declspec(noinline) long CEaxContext::SetReflectionsDelay(unsigned long slot, float value, bool record, bool clamp)
{
    if (ClampFloatProperty(&value, 0.0f, 0.3f, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_reflectionsDelayBase[slot] = value;
        m_reflectionsDelaySize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    float& stored = m_reverb[slot].flReflectionsDelay;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flReflectionsDelay = value;
            m_dirty |= 0x400;
        }
    }
    return 0;
}
// ---- 00c63750 (CEaxContext_SetReflectionsPan_00c63750.cpp) ----
inline bool IsAnyNaN(const EAXVECTOR& v)
{
    return _isnan(v.x) || _isnan(v.y) || _isnan(v.z);
}

__declspec(noinline) long CEaxContext::SetReflectionsPan(unsigned long slot, EAXVECTOR value)
{
    if (IsAnyNaN(value))
        return -3;
    EAXVECTOR& stored = m_reverb[slot].vReflectionsPan;
    if (memcmp(&stored, &value, sizeof(EAXVECTOR)) != 0)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.vReflectionsPan = value;
            m_dirty |= 0x800;
        }
    }
    return 0;
}
// ---- 00c63810 (CEaxContext_SetReverb_00c63810.cpp) ----
__declspec(noinline) long CEaxContext::SetReverb(unsigned long slot, long value, bool record, bool clamp)
{
    if (ClampLongProperty(&value, -10000, 2000, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_reverbBase[slot] = (float)value;
        m_reverbSize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    long& stored = m_reverb[slot].lReverb;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.lReverb = value;
            m_dirty |= 0x1000;
        }
    }
    return 0;
}
// ---- 00c638e0 (CEaxContext_SetReverbDelay_00c638e0.cpp) ----
__declspec(noinline) long CEaxContext::SetReverbDelay(unsigned long slot, float value, bool record, bool clamp)
{
    if (ClampFloatProperty(&value, 0.0f, 0.1f, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_reverbDelayBase[slot] = value;
        m_reverbDelaySize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    float& stored = m_reverb[slot].flReverbDelay;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flReverbDelay = value;
            m_dirty |= 0x2000;
        }
    }
    return 0;
}
// ---- 00c639a0 (CEaxContext_SetReverbPan_00c639a0.cpp) ----
__declspec(noinline) long CEaxContext::SetReverbPan(unsigned long slot, EAXVECTOR value)
{
    if (IsAnyNaN(value))
        return -3;
    EAXVECTOR& stored = m_reverb[slot].vReverbPan;
    if (memcmp(&stored, &value, sizeof(EAXVECTOR)) != 0)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.vReverbPan = value;
            m_dirty |= 0x4000;
        }
    }
    return 0;
}
// ---- 00c631e0 (CEaxContext_SetEnvironmentDiffusion_00c631e0.cpp) ----
__declspec(noinline) long CEaxContext::SetEnvironmentDiffusion(unsigned long slot, float value)
{
    if (value < 0.0f || value > 1.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flEnvironmentDiffusion;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flEnvironmentDiffusion = value;
            m_dirty |= 4;
        }
    }
    return 0;
}
// ---- 00c63a60 (CEaxContext_SetEchoTime_00c63a60.cpp) ----
__declspec(noinline) long CEaxContext::SetEchoTime(unsigned long slot, float value, bool record, bool clamp)
{
    if (ClampFloatProperty(&value, 0.075f, 0.25f, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_echoTimeBase[slot] = value;
        m_echoTimeSize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    float& stored = m_reverb[slot].flEchoTime;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flEchoTime = value;
            m_dirty |= 0x8000;
        }
    }
    return 0;
}
// ---- 00c63b20 (CEaxContext_SetEchoDepth_00c63b20.cpp) ----
__declspec(noinline) long CEaxContext::SetEchoDepth(unsigned long slot, float value)
{
    if (value < 0.0f || value > 1.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flEchoDepth;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flEchoDepth = value;
            m_dirty |= 0x10000;
        }
    }
    return 0;
}
// ---- 00c63bc0 (CEaxContext_SetModulationTime_00c63bc0.cpp) ----
__declspec(noinline) long CEaxContext::SetModulationTime(unsigned long slot, float value, bool record, bool clamp)
{
    if (ClampFloatProperty(&value, 0.04f, 4.0f, clamp) < 0)
    {
        m_lastError = -2;
        return -3;
    }
    if (record)
    {
        m_modulationTimeBase[slot] = value;
        m_modulationTimeSize[slot] = m_reverb[slot].flEnvironmentSize;
    }
    float& stored = m_reverb[slot].flModulationTime;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flModulationTime = value;
            m_dirty |= 0x20000;
        }
    }
    return 0;
}
// ---- 00c63c80 (CEaxContext_SetModulationDepth_00c63c80.cpp) ----
__declspec(noinline) long CEaxContext::SetModulationDepth(unsigned long slot, float value)
{
    if (value < 0.0f || value > 1.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flModulationDepth;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flModulationDepth = value;
            m_dirty |= 0x40000;
        }
    }
    return 0;
}
// ---- 00c63d20 (CEaxContext_SetSlotAirAbsorptionHF_00c63d20.cpp) ----
__declspec(noinline) long CEaxContext::SetSlotAirAbsorptionHF(unsigned long slot, float value)
{
    if (value < -100.0f || value > 0.0f)
    {
        m_lastError = -2;
        return -3;
    }
    if (m_reverb[slot].flAirAbsorptionHF != value)
    {
        m_reverb[slot].flAirAbsorptionHF = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flAirAbsorptionHF = value;
            m_dirty |= 0x80000;
        }
    }
    return 0;
}
// ---- 00c63ef0 (CEaxContext_SetRoomRolloffFactor_00c63ef0.cpp) ----
__declspec(noinline) long CEaxContext::SetRoomRolloffFactor(unsigned long slot, float value)
{
    if (value < 0.0f || value > 10.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flRoomRolloffFactor;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flRoomRolloffFactor = value;
            m_dirty |= 0x400000;
        }
    }
    return 0;
}
// ---- 00c63db0 (CEaxContext_SetSlotHFReference_00c63db0.cpp) ----
__declspec(noinline) long CEaxContext::SetSlotHFReference(unsigned long slot, float value)
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
// ---- 00c63e50 (CEaxContext_SetLFReference_00c63e50.cpp) ----
__declspec(noinline) long CEaxContext::SetLFReference(unsigned long slot, float value)
{
    if (value < 20.0f || value > 1000.0f)
    {
        m_lastError = -2;
        return -3;
    }
    float& stored = m_reverb[slot].flLFReference;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.flLFReference = value;
            m_dirty |= 0x200000;
        }
    }
    return 0;
}
// ---- 00c67a10 (CEaxContext_SetReverbFlags_00c67a10.cpp) ----
#pragma auto_inline(off)
__declspec(noinline) long CEaxContext::SetDecayTimeScale(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetReflectionsScale(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetReflectionsDelayScale(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetReverbScale(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetReverbDelayScale(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetDecayHFLimit(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetEchoTimeScale(unsigned long slot, long enable)
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

__declspec(noinline) long CEaxContext::SetModulationTimeScale(unsigned long slot, long enable)
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

#pragma auto_inline(on)

__declspec(noinline) long CEaxContext::SetReverbFlags(unsigned long slot, unsigned long flags)
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
// ---- 00c677c0 (00c677c0.cpp) ----
__declspec(noinline) long CEaxContext::SetEnvironmentSize(unsigned long slot, float value)
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
// ---- 00c63f90 (CEaxContext_SetDecayTimeScale_00c63f90.cpp) ----
// ---- 00c64020 (CEaxContext_SetReflectionsScale_00c64020.cpp) ----
// ---- 00c640b0 (CEaxContext_SetReflectionsDelayScale_00c640b0.cpp) ----
// ---- 00c64140 (CEaxContext_SetReverbScale_00c64140.cpp) ----
// ---- 00c641d0 (CEaxContext_SetReverbDelayScale_00c641d0.cpp) ----
// ---- 00c64260 (CEaxContext_SetDecayHFLimit_00c64260.cpp) ----
// ---- 00c642f0 (CEaxContext_SetEchoTimeScale_00c642f0.cpp) ----
// ---- 00c64380 (CEaxContext_SetModulationTimeScale_00c64380.cpp) ----
// ---- 00c632e0 / 00c63350 (landed as CLight::SetIntegerProperty2/3; the same bodies as SetRoom on lRoomHF / lRoomLF) ----
__declspec(noinline) long CEaxContext::SetRoomHF(unsigned long slot, long value)
{
    if (value < -10000 || value > 0)
    {
        m_lastError = -2;
        return -3;
    }
    long& stored = m_reverb[slot].lRoomHF;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.lRoomHF = value;
            m_dirty |= 0x10;
        }
    }
    return 0;
}
__declspec(noinline) long CEaxContext::SetRoomLF(unsigned long slot, long value)
{
    if (value < -10000 || value > 0)
    {
        m_lastError = -2;
        return -3;
    }
    long& stored = m_reverb[slot].lRoomLF;
    if (stored != value)
    {
        stored = value;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.lRoomLF = value;
            m_dirty |= 0x20;
        }
    }
    return 0;
}

long CEaxContext::SetEnvironment(unsigned long slot, unsigned long environment)
{
    EAXVECTOR zeroPan;
    zeroPan.x = 0.0f;
    zeroPan.y = 0.0f;
    zeroPan.z = 0.0f;
    if (environment > 26)
    {
        m_lastError = -2;
        return -3;
    }
    unsigned long& stored = m_reverb[slot].ulEnvironment;
    if (stored != environment)
    {
        stored = environment;
        if (m_primarySlot == slot && (m_slots[slot].ulFlags & 1))
        {
            m_committed.ulEnvironment = environment;
            m_dirty |= 1;
        }
        if (environment != 26)
        {
            const SEaxReverbPreset& preset = g_EaxReverbPresets[environment];
            if (SetRoom(slot, preset.lRoom) < 0) return 0;
            if (SetRoomHF(slot, preset.lRoomHF) < 0) return 0;
            if (SetRoomLF(slot, preset.lRoomLF) < 0) return 0;
            if (SetDecayTime(slot, preset.flDecayTime, true, false) < 0) return 0;
            if (SetDecayHFRatio(slot, preset.flDecayHFRatio) < 0) return 0;
            if (SetDecayLFRatio(slot, preset.flDecayLFRatio) < 0) return 0;
            if (SetReflections(slot, preset.lReflections, true, false) < 0) return 0;
            if (SetReflectionsDelay(slot, preset.flReflectionsDelay, true, false) < 0) return 0;
            if (SetReflectionsPan(slot, zeroPan) < 0) return 0;
            if (SetReverb(slot, preset.lReverb, true, false) < 0) return 0;
            if (SetReverbDelay(slot, preset.flReverbDelay, true, false) < 0) return 0;
            if (SetReverbPan(slot, zeroPan) < 0) return 0;
            if (SetEnvironmentDiffusion(slot, preset.flEnvironmentDiffusion) < 0) return 0;
            if (SetEchoTime(slot, preset.flEchoTime, true, false) < 0) return 0;
            if (SetEchoDepth(slot, preset.flEchoDepth) < 0) return 0;
            if (SetModulationTime(slot, preset.flModulationTime, true, false) < 0) return 0;
            if (SetModulationDepth(slot, preset.flModulationDepth) < 0) return 0;
            if (SetSlotAirAbsorptionHF(slot, preset.flAirAbsorptionHF) < 0) return 0;
            if (SetRoomRolloffFactor(slot, preset.flRoomRolloffFactor) < 0) return 0;
            if (SetSlotHFReference(slot, preset.flHFReference) < 0) return 0;
            if (SetLFReference(slot, preset.flLFReference) < 0) return 0;
            if (SetReverbFlags(slot, 0) < 0) return 0;
            if (SetEnvironmentSize(slot, preset.flEnvironmentSize) < 0) return 0;
            if (SetDecayTimeScale(slot, preset.lDecayTimeScale) < 0) return 0;
            if (SetReflectionsScale(slot, preset.lReflectionsScale) < 0) return 0;
            if (SetReflectionsDelayScale(slot, preset.lReflectionsDelayScale) < 0) return 0;
            if (SetReverbScale(slot, preset.lReverbScale) < 0) return 0;
            if (SetReverbDelayScale(slot, preset.lReverbDelayScale) < 0) return 0;
            if (SetDecayHFLimit(slot, preset.lDecayHFLimit) < 0) return 0;
            if (SetEchoTimeScale(slot, preset.lEchoTimeScale) < 0) return 0;
            SetModulationTimeScale(slot, preset.lModulationTimeScale);
        }
    }
    return 0;
}