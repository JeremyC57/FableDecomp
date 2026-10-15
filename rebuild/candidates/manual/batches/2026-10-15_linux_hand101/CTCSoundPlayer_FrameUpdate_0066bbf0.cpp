// CTCSoundPlayer::FrameUpdate() -- run the per-frame sound update only when the parent thing has the sound-player interface bit set
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066BBF0, length 18 bytes; vtable 0x0125B1C4 (CTCSoundPlayer) slot 9,
// the CTCBase FrameUpdate slot.
// Callees: the sound player's update @ 0x0066B370 (tail jump; unlabelled in the manifest).
// Layout: CThing keeps a 32-bit thing-component bitset whose word at +0x28 bit 26 is tested here (the inlined HasTC
// probe, see GOTCHAS wave 24b).
// Shape: a void forwarder as the last statement of an `if`: VC7.1 tail-jumps it.
struct CThing {
    unsigned char _pad[0x28];
    unsigned long m_tcBits28;                               // +0x28
};

struct CTCSoundPlayer {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    void UpdateSounds();                                    // 0x0066B370
    void FrameUpdate();
};

void CTCSoundPlayer::FrameUpdate()
{
    if (ParentThing->m_tcBits28 & 0x4000000)
        UpdateSounds();
}
