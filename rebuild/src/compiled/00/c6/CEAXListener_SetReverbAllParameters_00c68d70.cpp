// CEAXListener::SetReverbAllParameters(unsigned long slot, const _EAXREVERBPROPERTIES* value) -- EAX 4.0 reverb property EAXREVERB_ALLPARAMETERS of an FX slot: apply the whole record through the single-field setters in SDK field order (environment preset first, flags cleared, then every field, the flags last), stopping at the first negative result; always marks the all-parameters dirty bit 0x1000000 and returns the last result
// Leaf: ?SetReverbAllParameters@CEAXListener@@QAEJKPBU_EAXREVERBPROPERTIES@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C68D70, 542 bytes, thiscall `ret 8`.
// Identity (descriptive; the audio library has no PDB or RTTI): `this` is the landed CEAXListener (vtable 0x012B9340; FX slots
//   +0x84, _EAXREVERBPROPERTIES records +0xF4, committed reverb +0x340, dirty mask +0x3BC, last error +0x3D4). (1) The record
//   walked is _EAXREVERBPROPERTIES field by field (+0x00 ulEnvironment .. +0x6C ulFlags, 0x70 bytes) in the EAX 4.0 SDK order,
//   each field handed to the landed single-field setter of that field; (2) the sole caller 0x00C68F90 is the context's reverb
//   property dispatcher (the EAXREVERB_ALLPARAMETERS case), the reverb twin of the landed CEAXListener::SetAllParameters
//   0x00C67AE0 (EAXCONTEXT_ALLPARAMETERS). Dirty bit 24 is the record-level bit the committer tests.
// Callees (all CEAXListener, thiscall): SetEnvironment 0x00C68A50 (slot, ulEnvironment: a preset index <= 0x1A), SetReverbFlags
//   0x00C67A10 (first with 0, last with ulFlags), SetEnvironmentSize 0x00C677C0, SetEnvironmentDiffusion 0x00C631E0, SetRoom
//   0x00C63270, SetRoomHF 0x00C632E0, SetRoomLF 0x00C63350, SetDecayTime 0x00C633C0, SetDecayHFRatio 0x00C63480,
//   SetDecayLFRatio 0x00C63520, SetReflections 0x00C635C0, SetReflectionsDelay 0x00C63690, SetReflectionsPan 0x00C63750 (the
//   _EAXVECTOR by value), SetReverb 0x00C63810, SetReverbDelay 0x00C638E0, SetReverbPan 0x00C639A0, SetEchoTime 0x00C63A60,
//   SetEchoDepth 0x00C63B20, SetModulationTime 0x00C63BC0, SetModulationDepth 0x00C63C80, SetSlotAirAbsorptionHF 0x00C63D20,
//   SetSlotHFReference 0x00C63DB0, SetLFReference 0x00C63E50, SetRoomRolloffFactor 0x00C63EF0. The size-scaled setters take
//   (slot, value, record = true, clamp = false).
// Label corrections: the landed 0x00C632E0 / 0x00C63350 "CLight::SetIntegerProperty2/3" are CEAXListener::SetRoomHF / SetRoomLF
//   (_EAXREVERBPROPERTIES +0x10 lRoomHF / +0x14 lRoomLF, dirty bits 0x10 / 0x20 in field order, same class as the landed SetRoom
//   0x00C63270 whose note already calls the CLight labels propagated); the chunk label "ScriptCommand_SetLightAttributes" is a
//   propagated game name.
// Shape: retail does not reload ecx before SetRoom, SetRoomLF, SetDecayTime, SetDecayLFRatio, SetReflections, SetModulationTime,
//   SetSlotHFReference, SetLFReference and the final SetReverbFlags: VC7.1 knows the preceding callee keeps ecx only when its body
//   is in the same TU (GOTCHAS), so the landed bodies of those nine ecx-keeping setters sit here under __declspec(noinline), as the
//   landed SetAllParameters 0x00C67AE0 did for SetDistanceFactor. The by-value _EAXVECTOR arguments are built in the argument area
//   through eax/ecx/edx (a POD, no user copy constructor).
struct _GUID {                                     // GUID, 16 bytes
    unsigned long  Data1;
    unsigned short Data2, Data3;
    unsigned char  Data4[8];
};
struct _EAXVECTOR {
    float x, y, z;
};
struct _EAXFXSLOTPROPERTIES {                       // EAX 4.0, 0x1c bytes
    _GUID         guidLoadEffect;                  // +0x00
    long          lVolume;                         // +0x10
    long          lLock;                           // +0x14
    unsigned long ulFlags;                         // +0x18 bit 0: EAXFXSLOTFLAGS_ENVIRONMENT
};
struct _EAXREVERBPROPERTIES {                       // EAX 4.0, 0x70 bytes
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
    _EAXVECTOR     vReflectionsPan;                 // +0x2c
    long          lReverb;                         // +0x38
    float         flReverbDelay;                   // +0x3c
    _EAXVECTOR     vReverbPan;                      // +0x40
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
struct _EAXCONTEXTPROPERTIES {                      // EAX 4.0, 0x1c bytes
    _GUID         guidPrimaryFXSlotID;             // +0x00
    float         flDistanceFactor;                // +0x10
    float         flAirAbsorptionHF;               // +0x14
    float         flHFReference;                   // +0x18
};

class CEAXListener {                                // vtable 0x012B9340 (the vptr is not modelled here)
public:
    unsigned char        _vptr[4];                 // +0x000
    long                 m_primarySlot;            // +0x004 index of the primary FX slot, -1 = none
    unsigned char        _pad008[0x7c];            // +0x008
    _EAXFXSLOTPROPERTIES  m_slots[4];               // +0x084
    _EAXREVERBPROPERTIES  m_reverb[4];              // +0x0f4 one reverb record per FX slot
    _EAXCONTEXTPROPERTIES m_context;                // +0x2b4
    unsigned char        _pad2d0[0x70];            // +0x2d0
    _EAXREVERBPROPERTIES  m_committed;              // +0x340 the reverb the device renders
    unsigned char        _pad3b0[0xc];             // +0x3b0
    unsigned long        m_dirty;                  // +0x3bc committed-property dirty mask
    unsigned char        _pad3c0[0x14];            // +0x3c0
    long                 m_lastError;              // +0x3d4 -2 = invalid parameter

    long SetEnvironment(unsigned long slot, unsigned long environment);                  // 0x00C68A50
    long SetReverbFlags(unsigned long slot, unsigned long flags);                        // 0x00C67A10
    long SetEnvironmentSize(unsigned long slot, float value);                            // 0x00C677C0
    long SetEnvironmentDiffusion(unsigned long slot, float value);                       // 0x00C631E0 (body below)
    long SetRoom(unsigned long slot, long value);                                        // 0x00C63270
    long SetRoomHF(unsigned long slot, long value);                                      // 0x00C632E0 (body below)
    long SetRoomLF(unsigned long slot, long value);                                      // 0x00C63350 (body below)
    long SetDecayTime(unsigned long slot, float value, bool record, bool clamp);         // 0x00C633C0
    long SetDecayHFRatio(unsigned long slot, float value);                               // 0x00C63480 (body below)
    long SetDecayLFRatio(unsigned long slot, float value);                               // 0x00C63520 (body below)
    long SetReflections(unsigned long slot, long value, bool record, bool clamp);        // 0x00C635C0
    long SetReflectionsDelay(unsigned long slot, float value, bool record, bool clamp);  // 0x00C63690
    long SetReflectionsPan(unsigned long slot, _EAXVECTOR value);                         // 0x00C63750
    long SetReverb(unsigned long slot, long value, bool record, bool clamp);             // 0x00C63810
    long SetReverbDelay(unsigned long slot, float value, bool record, bool clamp);       // 0x00C638E0
    long SetReverbPan(unsigned long slot, _EAXVECTOR value);                              // 0x00C639A0
    long SetEchoTime(unsigned long slot, float value, bool record, bool clamp);          // 0x00C63A60
    long SetEchoDepth(unsigned long slot, float value);                                  // 0x00C63B20 (body below)
    long SetModulationTime(unsigned long slot, float value, bool record, bool clamp);    // 0x00C63BC0
    long SetModulationDepth(unsigned long slot, float value);                            // 0x00C63C80
    long SetSlotAirAbsorptionHF(unsigned long slot, float value);                        // 0x00C63D20 (body below)
    long SetSlotHFReference(unsigned long slot, float value);                            // 0x00C63DB0 (body below)
    long SetLFReference(unsigned long slot, float value);                                // 0x00C63E50
    long SetRoomRolloffFactor(unsigned long slot, float value);                          // 0x00C63EF0 (body below)
    long SetReverbAllParameters(unsigned long slot, const _EAXREVERBPROPERTIES* value);
};

// ---- the landed bodies of the setters retail's caller knows keep ecx (same TU) ----
__declspec(noinline) long CEAXListener::SetEnvironmentDiffusion(unsigned long slot, float value)   // 0x00C631E0
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
__declspec(noinline) long CEAXListener::SetRoomHF(unsigned long slot, long value)   // 0x00C632E0
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
__declspec(noinline) long CEAXListener::SetRoomLF(unsigned long slot, long value)   // 0x00C63350
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
__declspec(noinline) long CEAXListener::SetDecayHFRatio(unsigned long slot, float value)   // 0x00C63480
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
__declspec(noinline) long CEAXListener::SetDecayLFRatio(unsigned long slot, float value)   // 0x00C63520
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
__declspec(noinline) long CEAXListener::SetEchoDepth(unsigned long slot, float value)   // 0x00C63B20
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
__declspec(noinline) long CEAXListener::SetSlotAirAbsorptionHF(unsigned long slot, float value)   // 0x00C63D20
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
__declspec(noinline) long CEAXListener::SetSlotHFReference(unsigned long slot, float value)   // 0x00C63DB0
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
__declspec(noinline) long CEAXListener::SetRoomRolloffFactor(unsigned long slot, float value)   // 0x00C63EF0
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

long CEAXListener::SetReverbAllParameters(unsigned long slot, const _EAXREVERBPROPERTIES* value)
{
    long result;
    do
    {
        if ((result = SetEnvironment(slot, value->ulEnvironment)) < 0) break;
        if ((result = SetReverbFlags(slot, 0)) < 0) break;
        if ((result = SetEnvironmentSize(slot, value->flEnvironmentSize)) < 0) break;
        if ((result = SetEnvironmentDiffusion(slot, value->flEnvironmentDiffusion)) < 0) break;
        if ((result = SetRoom(slot, value->lRoom)) < 0) break;
        if ((result = SetRoomHF(slot, value->lRoomHF)) < 0) break;
        if ((result = SetRoomLF(slot, value->lRoomLF)) < 0) break;
        if ((result = SetDecayTime(slot, value->flDecayTime, true, false)) < 0) break;
        if ((result = SetDecayHFRatio(slot, value->flDecayHFRatio)) < 0) break;
        if ((result = SetDecayLFRatio(slot, value->flDecayLFRatio)) < 0) break;
        if ((result = SetReflections(slot, value->lReflections, true, false)) < 0) break;
        if ((result = SetReflectionsDelay(slot, value->flReflectionsDelay, true, false)) < 0) break;
        if ((result = SetReflectionsPan(slot, value->vReflectionsPan)) < 0) break;
        if ((result = SetReverb(slot, value->lReverb, true, false)) < 0) break;
        if ((result = SetReverbDelay(slot, value->flReverbDelay, true, false)) < 0) break;
        if ((result = SetReverbPan(slot, value->vReverbPan)) < 0) break;
        if ((result = SetEchoTime(slot, value->flEchoTime, true, false)) < 0) break;
        if ((result = SetEchoDepth(slot, value->flEchoDepth)) < 0) break;
        if ((result = SetModulationTime(slot, value->flModulationTime, true, false)) < 0) break;
        if ((result = SetModulationDepth(slot, value->flModulationDepth)) < 0) break;
        if ((result = SetSlotAirAbsorptionHF(slot, value->flAirAbsorptionHF)) < 0) break;
        if ((result = SetSlotHFReference(slot, value->flHFReference)) < 0) break;
        if ((result = SetLFReference(slot, value->flLFReference)) < 0) break;
        if ((result = SetRoomRolloffFactor(slot, value->flRoomRolloffFactor)) < 0) break;
        result = SetReverbFlags(slot, value->ulFlags);
    } while (0);
    m_dirty |= 0x1000000;
    return result;
}