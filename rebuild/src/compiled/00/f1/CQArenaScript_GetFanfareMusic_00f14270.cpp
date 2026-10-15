// CQ_ArenaScript::GetFanfareMusic(long round) -- the music track for the arena round's fanfare: rounds 1..9 map to tracks 0x24..0x2c, anything else to 0x23
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F14270, length 96 bytes + a 40-byte jump table (0x00F142D0).
// Callees: none. `this` is unused (`ret 4`).
// Shape: a ten-entry jump table over 0..9: an explicit `case 0` returning the same value as the fall-out return keeps
// 0 in the table range (a `case 0: default:` label pair drops it and biases the index); VC merges the two arms.
struct CQ_ArenaScript {
    long GetFanfareMusic(long round);
};

long CQ_ArenaScript::GetFanfareMusic(long round)
{
    switch (round) {
    case 0: return 0x23;
    case 1: return 0x24;
    case 2: return 0x25;
    case 3: return 0x26;
    case 4: return 0x27;
    case 5: return 0x28;
    case 6: return 0x29;
    case 7: return 0x2a;
    case 8: return 0x2b;
    case 9: return 0x2c;
    }
    return 0x23;
}