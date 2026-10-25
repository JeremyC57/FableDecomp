// OctaveRampStep(float from, float to, float seconds) -- the per-block multiplier of an exponential (octave) ramp: 2^((to - from) / blocks) over the ramp length in mix blocks, or the whole 2^(to - from) when the length rounds to zero blocks
// Leaf: ?OctaveRampStep@@YAMMMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59DE0, length 64 bytes; `ret` (__cdecl), the result on the x87 stack.
// Callees: SecondsToMixBlocks(float) @ 0x00C59B00, _CIpow (MSVCR71 import thunk 0x00BFEB40, IAT 0x01440108) x2.
// Globals: 2.0 (double) @ 0x01244480.
// Callers: the pitch-envelope update of the software voice (0x00C5A7EC / 0x00C5A7FE / 0x00C5A8DA / 0x00C5A8EC /
// 0x00C5AD9B / 0x00C5ADEE), the ramp family's (from, to, seconds) order of LinearRampStep 0x00C59D40.
// Label: the manifest's "Graphics_TimeToPixelsWrapper" is a propagated catalog name; descriptive name (GameCODA
// audio library, no PDB/RTTI).
// Shape: the block count lives in the `push ecx` slot (`mov [esp],eax ; fild [esp]`) and `to - from` is homed in
// the dead `to` slot; each arm returns a NAMED float local in its own scope: `return (float)pow(...)` directly is
// tail-called by VC7.1 (`add esp,4 ; jmp _CIpow`, 66 v 64) where retail calls then `pop ecx ; ret` (retail never
// tail-jumps to _CIpow: all 30 references are calls).
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)
long __cdecl SecondsToMixBlocks(float seconds);     // 0x00C59B00

float __cdecl OctaveRampStep(float from, float to, float seconds)
{
    long blocks = SecondsToMixBlocks(seconds);
    to -= from;
    if (blocks != 0) {
        float step = (float)pow(2.0, to / blocks);
        return step;
    }
    float whole = (float)pow(2.0, to);
    return whole;
}