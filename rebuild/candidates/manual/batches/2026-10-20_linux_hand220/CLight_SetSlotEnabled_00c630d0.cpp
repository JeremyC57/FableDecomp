// CLight::SetSlotEnabled(long slot, long enabled) -- switch an EAX effect slot on/off: on change, set/clear bit 0 of the slot flags; when the slot is the current one, enabling a reverb slot commits its EAX 3.0 listener properties (room level offset by the slot's RoomOffset, clamped at -10000 mB, dirty bits 0x5000000) and disabling silences the room (-10000), restores the default HF reference / air absorption and marks dirty 0x4180008; returns 0
// Leaf: ?SetSlotEnabled@CLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C630D0, length 248 bytes (`ret 8`, `this` moved to eax).
// Callees: none (memcmp of the slot GUID is the `repe cmpsd` intrinsic); data: the reverb effect GUID @ 0x012B94B8.
// Identity: the manifest label "CCharacter::SetSkillPropertyEx" is propagated; the body sits with the landed CLight
// EAX property setters (0x00C631E0 SetLightValue, 0x00C63270 SetIntegerProperty, which carry the same propagated
// "CLight" class name) and works on the same block: 0x1c-byte slots (here modelled from +0x84: GUID, RoomOffset +0x10,
// Flags +0x18 -- the landed files start them at +0x94 = RoomOffset), 0x70-byte EAX 3.0 EAXLISTENERPROPERTIES records
// from +0xf4 (lRoom +0xc, flAirAbsorptionHF +0x5c, flHFReference +0x60), the committed copy at +0x340, the dirty mask
// at +0x3bc. Local model (no PDB class); the two defaults at +0x2c8/+0x2cc are named from their destinations.
// Shape: `m_slots[slot]` indexed directly each time (a slot reference local moves the base into ebx); the old
// state is `(Flags & 1) == 1` (8-bit `dec ; neg ; sbb ; inc`); the enabling arm has its own `return 0` (separate
// epilogue, the disabling arm falls to the shared one); the disabling stores are written Room, HFReference,
// AirAbsorptionHF (the order the scheduler needs for retail's 0x3a0 / 0x34c / 0x39c store sequence).
#include <string.h>
struct SGuid { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const SGuid g_ReverbEffectGuid;            // 0x012B94B8

struct SEffectSlot {                              // 0x1c bytes from +0x84
    SGuid         EffectId;                       // +0x00
    long          RoomOffset;                     // +0x10
    long          _reserved14;                    // +0x14
    unsigned long Flags;                          // +0x18 bit 0: enabled
};
struct SListenerProperties {                      // EAX 3.0 EAXLISTENERPROPERTIES, 0x70 bytes
    unsigned long Environment;                    // +0x00
    float         EnvironmentSize;                // +0x04
    float         EnvironmentDiffusion;           // +0x08
    long          Room;                           // +0x0c
    unsigned char _body10[0x4c];
    float         AirAbsorptionHF;                // +0x5c
    float         HFReference;                    // +0x60
    unsigned char _body64[0xc];
};
struct CLight {
    unsigned char       _pad00[4];
    long                m_currentSlot;            // +0x04
    unsigned char       _pad08[0x7c];
    SEffectSlot         m_slots[3];               // +0x84
    unsigned char       _pad0d8[0x1c];
    SListenerProperties m_slotProperties[3];      // +0xf4
    unsigned char       _pad244[0x84];
    float               m_defaultAirAbsorptionHF; // +0x2c8
    float               m_defaultHFReference;     // +0x2cc
    unsigned char       _pad2d0[0x70];
    SListenerProperties m_committed;              // +0x340
    unsigned char       _pad3b0[0xc];
    unsigned long       m_dirty;                  // +0x3bc
    long SetSlotEnabled(long slot, long enabled);
};

long CLight::SetSlotEnabled(long slot, long enabled)
{
    if (enabled != ((m_slots[slot].Flags & 1) == 1)) {
        if (enabled) {
            m_slots[slot].Flags |= 1;
            if (m_currentSlot == slot) {
                if (memcmp(&m_slots[slot].EffectId, &g_ReverbEffectGuid, sizeof(SGuid)) == 0) {
                    m_committed = m_slotProperties[slot];
                    m_committed.Room += m_slots[slot].RoomOffset;
                    if (m_committed.Room < -10000)
                        m_committed.Room = -10000;
                    m_dirty |= 0x5000000;
                }
            }
            return 0;
        } else {
            m_slots[slot].Flags &= ~1;
            if (m_currentSlot == slot) {
                m_committed.Room = -10000;
                m_committed.HFReference = m_defaultHFReference;
                m_committed.AirAbsorptionHF = m_defaultAirAbsorptionHF;
                m_dirty |= 0x4180008;
            }
        }
    }
    return 0;
}
