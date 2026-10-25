// CEaxContext::SetSlotVolume(unsigned long slot, long volume) -- EAX 4.0 FX slot lVolume (-10000..0 mB, else last error -2 / result -3); on a change store it and, for the primary slot loaded with the reverb effect, recommit the room level (volume + the slot reverb's lRoom, floored at -10000; dirty bit 8)
// Leaf: ?SetSlotVolume@CEaxContext@@QAEJKJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C67720, 146 bytes (+ int3 padding), thiscall `ret 8`.
// Callees: none (the GUID compare is the `repe cmpsd` memcmp intrinsic); data: EAX_REVERB_EFFECT 0x012B94B8.
// Callers: 0x00C68A12 and 0x00C694BC (the slot property dispatchers).
// Naming: agent443's landed CEAXContext::SetSlotVolume, the class respelled CEaxContext.
// Class (sweep1, 2026-10-25; FINDINGS chunk 442 "CLight 0xC630D0/0xC631E0/0xC63270, CSkillTable_00c67470 and CDrawGuildSeal
//   labels are the EAX context", chunk 443 "0xC630D0 "CLight::SetSlotEnabled" / 0xC67470 = CEAXContext members"): agent442's
//   software EAX 4.0 context CEaxContext (vtable 0x012B9340, destructor 0x00C60930, 25 landed rows): primary FX slot +0x04,
//   four EAXFXSLOTPROPERTIES at +0x84, one EAXREVERBPROPERTIES per slot at +0xf4, EAXCONTEXTPROPERTIES at +0x2b4, the
//   committed reverb +0x340, dirty mask +0x3bc, last error +0x3d4. agent443 spelled the same object CEAXContext (three
//   rows, renamed here to the majority spelling). Ranges and fields are the EAX 4.0 SDK's; 0x012B94B8 is EAX_REVERB_EFFECT
//   (bound under that name by agent442's 0x00C62FF0). Descriptive names (libpk: no PDB/RTTI).
// Shape: the `||` range test puts the error block at the end; `volume + lRoom` (volume first: `add ebx,edx`) is stored
//   and re-tested from the register; memcmp(...) == 0 inside a nested if after the primary-slot test.
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
extern const SGuid EAX_REVERB_EFFECT;                // 0x012B94B8 {0CF95C8F-A3CC-4849-B0B6-832ECC1822DF}

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

    long SetSlotVolume(unsigned long slot, long volume);
};

long CEaxContext::SetSlotVolume(unsigned long slot, long volume)
{
    if (volume < -10000 || volume > 0) {
        m_lastError = -2;
        return -3;
    }
    if (volume != m_slots[slot].lVolume) {
        m_slots[slot].lVolume = volume;
        if (m_primarySlot == slot) {
            if (memcmp(&m_slots[slot].guidLoadEffect, &EAX_REVERB_EFFECT, sizeof(SGuid)) == 0) {
                m_committed.lRoom = volume + m_reverb[slot].lRoom;
                if (m_committed.lRoom < -10000)
                    m_committed.lRoom = -10000;
                m_dirty |= 8;
            }
        }
    }
    return 0;
}
