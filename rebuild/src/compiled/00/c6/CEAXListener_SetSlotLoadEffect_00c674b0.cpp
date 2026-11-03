// CEAXListener::SetSlotLoadEffect(unsigned long slot, _GUID effect) -- EAX 4.0 FX slot guidLoadEffect: slots 0 and 1 are fixed (last error -1, result -3); loading EAX_REVERB_EFFECT resets the slot's reverb to the EAX 4.0 generic defaults and, on the primary environment slot, re-commits it with the slot volume folded into lRoom; loading EAX_NULL_GUID unloads it (the primary slot's committed air absorption and HF reference revert to the context's, lRoom to -10000); any other GUID is last error -4, result -3
// Leaf: ?SetSlotLoadEffect@CEAXListener@@QAEJKU_GUID@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C674B0, 618 bytes, thiscall `ret 14h` (the slot index + the 16-byte GUID by value).
// Identity: the slot property dispatcher 0x00C689B0 (landed as "CSkillTable_00c67470::SetSkillPropertiesEx" calling
//   "SetSkillGUID": NAME_FIXES) calls it with the slot record's guidLoadEffect beside SetSlotVolume 0x00C67720,
//   SetSlotLock 0x00C67470 and SetSlotFlags 0x00C67430 (all landed in CEAXListener); the GUIDs are EAX_REVERB_EFFECT
//   0x012B94B8 {0CF95C8F-A3CC-4849-B0B6-832ECC1822DF} and EAX_NULL_GUID 0x012B9438; the defaults are the EAX 4.0 SDK
//   EAXREVERB_DEFAULT* values (environment GENERIC 0, size 7.5, diffusion 1.0, room -1000, roomHF -100, roomLF 0,
//   decay 1.49, decayHF 0.83, decayLF 1.0, reflections -2602 / 0.007, reverb 200 / 0.011, echo 0.25 / 0, modulation
//   0.25 / 0, air absorption -5.0, HF 5000, LF 250, rolloff 0, flags 0x3f). Descriptive name (libpk: no PDB/RTTI); the
//   catalog "CCharacter_AddSkill" is propagated.
// Callees: none (two `repe cmpsd` = intrinsic memcmp through the GUID `operator==`; `rep movsd` = the 0x70-byte reverb
//   record copy).
// Layout: the landed CEAXListener (primary FX slot +0x04, _EAXFXSLOTPROPERTIES[4] +0x84, _EAXREVERBPROPERTIES[4] +0xf4,
//   _EAXCONTEXTPROPERTIES +0x2b4, committed reverb +0x340, dirty mask +0x3bc, last error +0x3d4).
// Shape: `if (reverb) {..} else if (null) {..} else { error -4 }` with ONE shared `return 0` (the reverb arm's dirty
//   store gets a duplicated epilogue with the pops interleaved, the null arm falls into the common tail; separate
//   `return 0`s move the null arm to the end); the reverb defaults are member-wise `m_reverb[slot].x = ..` stores in
//   the SDK order (a local record pointer materialises `+0xf4` in esi instead of folding it into the displacements; VC7.1
//   groups the zero stores through ebp, the 1.0f and 0.25f pairs through ecx, folds +0x5c into `(slot + 3) * 0x70`);
//   the slot GUID is assigned from the global, not the argument; the null arm stores air absorption, lRoom, HF reference,
//   dirty in that order (the order decides which store the scheduler sinks).
#include <string.h>
#pragma intrinsic(memcmp)

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
    long          lVolume;                         // +0x10 -10000..0 mB
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
    unsigned long ulFlags;                         // +0x6c EAXREVERBFLAGS_* (bits 0..7)
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
    long                 m_lastError;              // +0x3d4 -1 = reserved slot, -4 = unknown effect

    long SetSlotLoadEffect(unsigned long slot, _GUID effect);
};
extern "C" const _GUID EAX_REVERB_EFFECT;                // 0x012B94B8 {0CF95C8F-A3CC-4849-B0B6-832ECC1822DF}
extern "C" const _GUID EAX_NULL_GUID;                    // 0x012B9438 {00000000-0000-0000-0000-000000000000}
inline bool operator==(const _GUID& a, const _GUID& b) { return !memcmp(&a, &b, sizeof(_GUID)); }

long CEAXListener::SetSlotLoadEffect(unsigned long slot, _GUID effect)
{
    if (slot == 0 || slot == 1) {
        m_lastError = -1;
        return -3;
    }
    if (effect == EAX_REVERB_EFFECT) {
        m_reverb[slot].ulEnvironment = 0;
        m_reverb[slot].flEnvironmentSize = 7.5f;
        m_reverb[slot].flEnvironmentDiffusion = 1.0f;
        m_reverb[slot].lRoom = -1000;
        m_reverb[slot].lRoomHF = -100;
        m_reverb[slot].lRoomLF = 0;
        m_reverb[slot].flDecayTime = 1.49f;
        m_reverb[slot].flDecayHFRatio = 0.83f;
        m_reverb[slot].flDecayLFRatio = 1.0f;
        m_reverb[slot].lReflections = -2602;
        m_reverb[slot].flReflectionsDelay = 0.007f;
        m_reverb[slot].vReflectionsPan.x = 0.0f;
        m_reverb[slot].vReflectionsPan.y = 0.0f;
        m_reverb[slot].vReflectionsPan.z = 0.0f;
        m_reverb[slot].lReverb = 200;
        m_reverb[slot].flReverbDelay = 0.011f;
        m_reverb[slot].vReverbPan.x = 0.0f;
        m_reverb[slot].vReverbPan.y = 0.0f;
        m_reverb[slot].vReverbPan.z = 0.0f;
        m_reverb[slot].flEchoTime = 0.25f;
        m_reverb[slot].flEchoDepth = 0.0f;
        m_reverb[slot].flModulationTime = 0.25f;
        m_reverb[slot].flModulationDepth = 0.0f;
        m_reverb[slot].flAirAbsorptionHF = -5.0f;
        m_reverb[slot].flHFReference = 5000.0f;
        m_reverb[slot].flLFReference = 250.0f;
        m_reverb[slot].flRoomRolloffFactor = 0.0f;
        m_reverb[slot].ulFlags = 0x3f;
        m_slots[slot].guidLoadEffect = EAX_REVERB_EFFECT;
        if (m_primarySlot == (long)slot && (m_slots[slot].ulFlags & 1)) {
            m_committed = m_reverb[slot];
            m_committed.lRoom += m_slots[slot].lVolume;
            if (m_committed.lRoom < -10000)
                m_committed.lRoom = -10000;
            m_dirty |= 0x1000000;
        }
    } else if (effect == EAX_NULL_GUID) {
        m_slots[slot].guidLoadEffect = EAX_NULL_GUID;
        if (m_primarySlot == (long)slot) {
            m_committed.flAirAbsorptionHF = m_context.flAirAbsorptionHF;
            m_committed.lRoom = -10000;
            m_committed.flHFReference = m_context.flHFReference;
            m_dirty |= 0x180008;
        }
    } else {
        m_lastError = -4;
        return -3;
    }
    return 0;
}