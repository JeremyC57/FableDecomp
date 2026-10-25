// LinearRampStep(float from, float to, float seconds) -- the per-block increment of a linear envelope ramp: (to - from) divided by the ramp length in mix blocks, or the whole step when the length rounds to zero blocks
// Leaf: ?LinearRampStep@@YAMMMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59D40, length 60 bytes (+ int3 padding); `ret` (__cdecl), the result
// on the x87 stack.
// Callee: SecondsToMixBlocks(float) @ 0x00C59B00 (seconds * 44100 / block frames, rounded; retail inline asm).
// Callers: the GameCODA software-voice envelope update 0x00C59E40.. (attack: `LinearRampStep(0.0f, 1.0f, attack)`
// into the per-block delta +0x34, twice), so the arguments are (start level, end level, seconds).
// Label: the manifest's "Animation_ComputeFrameRate" is a propagated catalog name; this is the GameCODA (Sensaura
// "CODA") audio library beside libpk (no PDB/RTTI): descriptive name.
// Shape: the block count is converted back to float into the dead `seconds` slot (`mov [esp+10h],eax ; fild`): the
// source reuses the parameter (`seconds = (float)SecondsToMixBlocks(seconds)`); a named local takes a fresh slot.
// `!= 0.0f` is `fucompp ; test ah,44h ; jnp`, the two returns each recompute `to - from`.
long __cdecl SecondsToMixBlocks(float seconds);     // 0x00C59B00

float __cdecl LinearRampStep(float from, float to, float seconds)
{
    seconds = (float)SecondsToMixBlocks(seconds);
    if (seconds != 0.0f)
        return (to - from) / seconds;
    return to - from;
}