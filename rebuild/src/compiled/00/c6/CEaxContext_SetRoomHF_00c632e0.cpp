// CEaxContext::SetRoomHF(unsigned long slot, long value) -- validate a value in millibels (-10000..0, else last error -2, result -3), store it in the slot's reverb record (RoomHF) when it changed and, when that slot is the current, live one, commit it (m_committedRoomHF) and mark dirty bit 0x10; returns 0
// Leaf: ?SetRoomHF@CEaxContext@@QAEJKJ@Z
// Name (naming wave 20, chunk 686): the EAX 4.0 reverb-slot property setter the landed CEaxContext::SetEnvironment 0x00C68A50 calls 2nd in its
//   preset walk (lRoomHF / lRoomLF of the EAXREVERBPROPERTIES block: the -10000..0 millibel range, dirty bit 0x10); descriptive member name (libpk, no
//   PDB); the class is the landed CEaxContext (SetEnvironment / SetDecayTime 0x00C633C0 share its `this`); was the propagated CLight::SetIntegerProperty2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C632E0, length 112 bytes.
// Callees: none.
// Shape: the range test (`jl`/`jg`) jumps to the failure block at the end; the stored value is reached through a
// reference local; `stored != value` keeps the member on the left of the compare.
// Local model (no PDB class carries these offsets): an EAX/DirectSound-style property block like the landed
// CEntity::SetVectorProperty_XY (0x00C647B0) -- per-slot 0x1c-byte slots at +0x94 (base value +0x00, flags +0x08
// with bit 0 = slot live), per-slot 0x70-byte property records from +0xfc (Value, Level, RoomHF, RoomLF), the
// committed copies of the current slot's properties at +0x348.., the dirty mask at +0x3bc and the last error at
// +0x3d4 (-2 = invalid parameter; the setters then return -3). Array lengths are bounded by the next member.
struct SSlotRecord {                                // 0x1c bytes
    long          BaseLevel;                       // +0x00
    unsigned char _pad04[4];
    unsigned char Flags;                           // +0x08 bit 0: live
    unsigned char _pad09[0x13];
};
struct SSlotProperties {                          // 0x70 bytes
    float         Value;                           // +0x00
    long          Level;                           // +0x04
    long          RoomHF;                          // +0x08
    long          RoomLF;                          // +0x0c
    unsigned char _pad10[0x60];
};
class CEaxContext { public:
    unsigned char    _pad00[4];
    long             m_currentSlot;               // +0x04
    unsigned char    _pad08[0x8c];
    SSlotRecord      m_slotRecords[3];                   // +0x94
    unsigned char    _pad0e8[0x14];
    SSlotProperties  m_slotProperties[5];                  // +0xfc
    unsigned char    _pad32c[0x1c];
    float            m_committedValue;             // +0x348
    long             m_committedLevel;             // +0x34c
    long             m_committedRoomHF;            // +0x350
    long             m_committedRoomLF;            // +0x354
    unsigned char    _pad358[0x64];
    unsigned long    m_dirty;                      // +0x3bc
    unsigned char    _pad3c0[0x14];
    long             m_lastError;                  // +0x3d4
    long SetRoomHF(unsigned long slot, long value);
};

long CEaxContext::SetRoomHF(unsigned long slot, long value)
{
    if (value < -10000 || value > 0)
    {
        m_lastError = -2;
        return -3;
    }
    long& stored = m_slotProperties[slot].RoomHF;
    if (stored != value)
    {
        stored = value;
        if (m_currentSlot == slot && (m_slotRecords[slot].Flags & 1))
        {
            m_committedRoomHF = value;
            m_dirty |= 0x10;
        }
    }
    return 0;
}