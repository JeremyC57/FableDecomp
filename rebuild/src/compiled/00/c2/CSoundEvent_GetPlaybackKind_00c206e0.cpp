// CSoundEvent::GetPlaybackKind() -- classify the event state: waiting (2..4) -> 0, playing on a virtual player (5) -> 1, playing on a physical player (6) -> 2, otherwise -1
// Leaf: ?GetPlaybackKind@CSoundEvent@@QAEJXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C206E0, 64 bytes (5-entry jump table), thiscall.
// Identity: libpk (no PDB/RTTI): descriptive name. Only caller: slot 10 of the CSoundEvent vtable 0x012B3674 (thunk 0x00C213B0).
//   The state names come from the landed describer 0x00C21040 (2..4 waiting, 5 virtual player, 6 physical player).
// Shape: switch biased by 2, each case its own return, default -1.
struct CSoundEvent {                             // 0xB4-byte libpk sound event, vtable 0x012B3674 (no RTTI)
    unsigned char  _pad00[0xac];
    unsigned short m_state;                     // +0xAC
    long GetPlaybackKind();
};

long CSoundEvent::GetPlaybackKind()
{
    switch (m_state) {
    case 2: case 3: case 4: return 0;
    case 5: return 1;
    case 6: return 2;
    }
    return -1;
}