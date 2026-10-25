// SpeakerToChannelIndex(unsigned long speaker) -- map one WAVEFORMATEXTENSIBLE speaker bit to the GameCODA mixer's channel slot: front left 0, front right 1, back left 2, back right 3, front centre 4, low frequency 5, anything else -1
// Leaf: ?SpeakerToChannelIndex@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5C980, length 124 bytes including the 7-entry jump table and the
// 32-entry byte index table; `ret` (__cdecl).
// Callers: 0x00C5CADA and 0x00C5CB4A (the speaker-configuration code of the output device). No callees.
// Label: the manifest's "Direction_MapBitmaskToIndex" describes the shape; the name follows the SPEAKER_* bits
// (1 FL, 2 FR, 4 FC, 8 LFE, 0x10 BL, 0x20 BR) and the six-channel accumulator order L, R, BL, BR, C, LFE.
// Shape: VC7.1 lowers the sparse switch to `dec ; cmp 1Fh ; ja default ; movzx eax, byte table[eax] ; jmp table[eax*4]`
// and lays out the case bodies in SOURCE order, so the cases are written in the order of retail's bodies (return
// values 0..5); the jump-table targets (relocations, masked by the grader) were checked by hand against retail:
// entries 1, 2, 4, 8, 0x10, 0x20, default -> +0x18, +0x1B, +0x2D, +0x33, +0x21, +0x27, +0x39 in both.
long __cdecl SpeakerToChannelIndex(unsigned long speaker)
{
    switch (speaker) {
    case 0x01: return 0;    // SPEAKER_FRONT_LEFT
    case 0x02: return 1;    // SPEAKER_FRONT_RIGHT
    case 0x10: return 2;    // SPEAKER_BACK_LEFT
    case 0x20: return 3;    // SPEAKER_BACK_RIGHT
    case 0x04: return 4;    // SPEAKER_FRONT_CENTER
    case 0x08: return 5;    // SPEAKER_LOW_FREQUENCY
    }
    return -1;
}