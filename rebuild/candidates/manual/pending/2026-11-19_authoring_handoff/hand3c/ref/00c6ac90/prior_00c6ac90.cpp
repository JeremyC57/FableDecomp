// CAudioFilterStage::Process(short* source, long gain, long step) -- marks the stage active (+8) and runs the 16-sample MMX IIR filter from the source into the stage's accumulator (+0x21c) with the gain and step
// Grade: DIFFER(39v38) -- lane gap (LTCG custom calling convention): retail loads the gain into ecx (`mov ecx,[esp+10h]`) for a callee that also takes four caller-cleaned stack arguments (`add esp,10h`): 0x00C6AAE0 reads ecx as a parameter (`mov word ptr [ebp-30h],cx`) and returns with a plain `ret`, a /GL whole-program convention no source-level declaration reproduces (__cdecl pushes the gain, __fastcall makes the callee pop). Tried: __cdecl callee (one grade).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6AC90, length 54 bytes (+ int3 padding).
// Callees: the MMX IIR sample filter @ 0x00C6AAE0 (manifest "Audio_ProcessSamples_IIRFilter": /GS frame,
// `and esp,-16` alignment, a 16-iteration pmulhw/paddsw loop). Callers 0x00C54EF2 and 0x00C5C843 (audio
// channel updates that pass `this` = [[0x0143BC2C]+0x17C]).
// The chunk label "CTCCarrying::SetAllCarriedThingsAsBypassFilter" is wrong: this is the audio module
// (0x00C5..0x00C6 region), `ret 0Ch` with three arguments.
// Layout: a flag byte +8, the accumulator buffer pointer +0x21c.
struct CAudioFilterStage {
    unsigned char _pad[8];
    unsigned char Active;                                   // +0x008
    unsigned char _pad9[0x213];
    long*         Accumulator;                              // +0x21c
    void Process(short* source, long gain, long step);
};

extern "C" void __cdecl ProcessSamplesIIRFilter(long gain, short* source, long count, long* accumulator, long step);   // 0x00C6AAE0

void CAudioFilterStage::Process(short* source, long gain, long step)
{
    Active = 1;
    ProcessSamplesIIRFilter(gain, source, 16, Accumulator, step);
}
