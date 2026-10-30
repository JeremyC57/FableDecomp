// CEaxContext::SetAllParameters(const EAXCONTEXTPROPERTIES* value) -- EAX 4.0 context property EAXCONTEXT_ALLPARAMETERS: validate the record (flAirAbsorptionHF -100..0, flHFReference 1000..20000, guidPrimaryFXSlotID one of the four FX slots or EAX_NULL_GUID; else last error -2 and result -3), then apply it through the four single-field setters, stopping at the first negative result
// Leaf: ?SetAllParameters@CEaxContext@@QAEJPBUEAXCONTEXTPROPERTIES@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C67AE0, 288 bytes, thiscall `ret 4`; the next function starts at once (no padding).
// Identity (descriptive; libpk has no PDB or RTTI): its only caller 0x00C68400 is the context's EAX property dispatcher,
//   whose neighbouring cases call the single-field setters with the same `this` (SetPrimaryFXSlotID 0x00C64420 with a
//   16-byte by-value GUID, SetDistanceFactor 0x00C645D0, SetAirAbsorptionHF 0x00C64630, SetHFReference 0x00C646E0 -- the
//   landed CEaxContext rows); this row validates EAXCONTEXTPROPERTIES (+0x10/+0x14/+0x18 floats, the GUID at +0) against
//   the EAX 4.0 SDK ranges (float pool -100.0f 0x01275E8C, 0.0f 0x0122DEDC, 1000.0f 0x01252370, 20000.0f 0x012B89C0) and
//   the slot GUIDs EAXPROPERTYID_EAX40_FXSlot0..3 0x012B9468.. / EAX_NULL_GUID 0x012B9438, then calls the four setters.
//   The source-side twin is agent443's SetAllSourceProperties 0x00C67C00. The chunk label is a propagated game name.
// Callees: SetPrimaryFXSlotID 0x00C64420, SetDistanceFactor 0x00C645D0, SetAirAbsorptionHF 0x00C64630, SetHFReference
//   0x00C646E0 (all CEaxContext, thiscall `ret 4` / `ret 10h`).
// Shape: retail does not reload ecx before SetAirAbsorptionHF: VC7.1 knows SetDistanceFactor keeps ecx only when its body is
//   in the same TU (GOTCHAS: same-TU callees are checked for register preservation), so the landed body of 0x00C645D0 sits
//   here under __declspec(noinline). The GUID tests are the inline memcmp `operator==` (repe cmpsd, eax/edx alternate).
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

    long SetPrimaryFXSlotID(SGuid id);             // 0x00C64420
    long SetDistanceFactor(float value);           // 0x00C645D0 (body below: it keeps ecx)
    long SetAirAbsorptionHF(float value);          // 0x00C64630
    long SetHFReference(float value);              // 0x00C646E0
    long SetAllParameters(const EAXCONTEXTPROPERTIES* value);
};
extern const SGuid EAXPROPERTYID_EAX40_FXSlot0;      // 0x012B9468 {C4D79F1E-F1AC-436B-A81D-A738E7045469}
extern const SGuid EAXPROPERTYID_EAX40_FXSlot1;      // 0x012B9478 {08C00E96-74BE-4491-93AA-E8AD35A49117}
extern const SGuid EAXPROPERTYID_EAX40_FXSlot2;      // 0x012B9488 {1D433B88-F0F6-4637-919F-60E7E06B5EDD}
extern const SGuid EAXPROPERTYID_EAX40_FXSlot3;      // 0x012B9498 {EFFF08EA-C7D8-44AB-93AD-6DBD5F910064}
extern const SGuid EAX_NULL_GUID;                    // 0x012B9438 {00000000-0000-0000-0000-000000000000}
inline bool operator==(const SGuid& a, const SGuid& b) { return !memcmp(&a, &b, sizeof(SGuid)); }


__declspec(noinline) long CEaxContext::SetDistanceFactor(float value)      // 0x00C645D0 (the landed body)
{
    if (value < 1.175494351e-38f || value > 3.402823466e+38f)
    {
        m_lastError = -2;
        return -3;
    }
    if (value != m_context.flDistanceFactor)
    {
        m_context.flDistanceFactor = value;
        m_dirty |= 0x2000000;
    }
    return 0;
}

long CEaxContext::SetAllParameters(const EAXCONTEXTPROPERTIES* value)
{
    if (value->flAirAbsorptionHF < -100.0f || value->flAirAbsorptionHF > 0.0f
        || value->flHFReference < 1000.0f || value->flHFReference > 20000.0f
        || !(value->guidPrimaryFXSlotID == EAXPROPERTYID_EAX40_FXSlot0
             || value->guidPrimaryFXSlotID == EAXPROPERTYID_EAX40_FXSlot1
             || value->guidPrimaryFXSlotID == EAXPROPERTYID_EAX40_FXSlot2
             || value->guidPrimaryFXSlotID == EAXPROPERTYID_EAX40_FXSlot3
             || value->guidPrimaryFXSlotID == EAX_NULL_GUID))
    {
        m_lastError = -2;
        return -3;
    }
    long result = SetPrimaryFXSlotID(value->guidPrimaryFXSlotID);
    if (result < 0)
        return result;
    result = SetDistanceFactor(value->flDistanceFactor);
    if (result < 0)
        return result;
    result = SetAirAbsorptionHF(value->flAirAbsorptionHF);
    if (result < 0)
        return result;
    return SetHFReference(value->flHFReference);
}