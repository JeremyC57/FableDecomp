// CEaxContext::SetPrimaryFXSlotID(SGuid id) -- EAX 4.0 context property EAXCONTEXT_PRIMARYFXSLOTID: map the GUID to a slot index (EAXPROPERTYID_EAX40_FXSlot0..3 -> 0..3, EAX_NULL_GUID -> -1; anything else sets last error -2 and returns -3); on a change store the GUID and index (dirty bit 0x4000000), then recommit the device reverb from the new primary slot when it holds EAX_REVERB_EFFECT with the environment flag (record copy, room + slot volume floored at -10000, dirty 0x1000000), else mute the room and recommit the context's air absorption / HF reference (dirty 0x180008)
// Grade: DIFFER(424v426) -- dirty mask register only: retail computes `m_dirty | 0x4000000` in esi (while ebx still holds &m_context for the GUID copy) and then moves it to ebx (`mov ebx,esi`), ebp being the memcmp zero; ours keeps the local in ebp from the load (ebx is the memcmp zero). Everything else (GUID chain, slot pointer spilled to the dead id slot, record copy, clamp) matches.
// Leaf: ?SetPrimaryFXSlotID@CEaxContext@@QAEJUSGuid@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C64420, 426 bytes, thiscall `ret 10h` (the 16-byte GUID by value). No callees; the GUID
//   tests are the inline memcmp `repe cmpsd` (the first one's zeroed edx doubles as slot index 0); the record copy is `rep movsd`
//   (0x1c dwords). Data: EAXPROPERTYID_EAX40_FXSlot0..3 0x012B9468/78/88/98, EAX_NULL_GUID 0x012B9438, EAX_REVERB_EFFECT 0x012B94B8.
// Identity (descriptive method name; libpk has no PDB or RTTI): the landed CEaxContext::SetAllParameters 0x00C67AE0 calls this
//   address with the record's guidPrimaryFXSlotID and binds it `?SetPrimaryFXSlotID@CEaxContext@@QAEJUSGuid@@@Z`; the other caller
//   0x00C68400 is the context property dispatcher. The GUID values are the EAX 4.0 SDK's (eax.h), bound under these names by
//   the landed 0x00C62FF0 / 0x00C67AE0. Model: the landed CEaxContext rows (agent442/443 layout).
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

    long SetPrimaryFXSlotID(SGuid id);
};
extern const SGuid EAXPROPERTYID_EAX40_FXSlot0;      // 0x012B9468 {C4D79F1E-F1AC-436B-A81D-A738E7045469}
extern const SGuid EAXPROPERTYID_EAX40_FXSlot1;      // 0x012B9478 {08C00E96-74BE-4491-93AA-E8AD35A49117}
extern const SGuid EAXPROPERTYID_EAX40_FXSlot2;      // 0x012B9488 {1D433B88-F0F6-4637-919F-60E7E06B5EDD}
extern const SGuid EAXPROPERTYID_EAX40_FXSlot3;      // 0x012B9498 {EFFF08EA-C7D8-44AB-93AD-6DBD5F910064}
extern const SGuid EAX_NULL_GUID;                    // 0x012B9438 {00000000-0000-0000-0000-000000000000}
extern const SGuid EAX_REVERB_EFFECT;                // 0x012B94B8 {0CF95C8F-A3CC-4849-B0B6-832ECC1822DF}
inline bool operator==(const SGuid& a, const SGuid& b) { return !memcmp(&a, &b, sizeof(SGuid)); }

long CEaxContext::SetPrimaryFXSlotID(SGuid id)
{
    long slot;
    if (id == EAXPROPERTYID_EAX40_FXSlot0)
        slot = 0;
    else if (id == EAXPROPERTYID_EAX40_FXSlot1)
        slot = 1;
    else if (id == EAXPROPERTYID_EAX40_FXSlot2)
        slot = 2;
    else if (id == EAXPROPERTYID_EAX40_FXSlot3)
        slot = 3;
    else if (id == EAX_NULL_GUID)
        slot = -1;
    else
    {
        m_lastError = -2;
        return -3;
    }
    if (!(m_context.guidPrimaryFXSlotID == id))
    {
        unsigned long dirty = m_dirty | 0x4000000;
        m_context.guidPrimaryFXSlotID = id;
        m_primarySlot = slot;
        m_dirty = dirty;
        if (slot != -1 && m_slots[slot].guidLoadEffect == EAX_REVERB_EFFECT && (m_slots[slot].ulFlags & 1))
        {
            m_committed = m_reverb[slot];
            m_committed.lRoom += m_slots[slot].lVolume;
            if (m_committed.lRoom < -10000)
                m_committed.lRoom = -10000;
            m_dirty = dirty | 0x1000000;
        }
        else
        {
            m_committed.lRoom = -10000;
            m_committed.flAirAbsorptionHF = m_context.flAirAbsorptionHF;
            m_committed.flHFReference = m_context.flHFReference;
            m_dirty = dirty | 0x180008;
        }
    }
    return 0;
}
