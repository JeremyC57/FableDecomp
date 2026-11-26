// CSalRenderDisable::MixFadeOut(const short* source, const FilterPan* filter, int* accumulator) -- fade a source out over one 128-sample block: accumulator[i] += (source[i] * gain) >> 15 with the Q15 gain falling from the record's gain by its step each sample (nothing when the gain is 0); byte-identical copy of 0x00C6DAD0 in the 0x012BEDB8 effect's TU
// Leaf: ?MixFadeOut@CSalRenderDisable@@CAXPBFPBUFilterPan@@PAH@Z
// Naming (NF463, queue nf452): the GameCODA 2.10 SDK private static CSalRenderDisable::MixFadeOut(const short*, const FilterPan*, int*), CODAApi.lib
//   Release/SalRenderDisable.obj (?MixFadeOut@CSalRenderDisable@@CAXPBFPBUFilterPan@@PAH@Z): same instruction count and plain ret as retail, and retail's object slice order matches the SDK object
//   order (NF452 evidence). Was the descriptive free function ?MixGainRampDown@@YAXPBFPBUSGainRamp@@PAJ@Z over a local SGainRamp (the SDK FilterPan: gain +0,
//   gainDelta +2) with a long* accumulator (int and long are both 32-bit: same code). On the shared engine/CSalRenderDisable.h with the add-only
//   opt-in FABLE_RETAIL_CSALRENDERDISABLE_MIXFADE (declarations of the private statics). The byte twin 0x00C6DAD0 (CSalRenderPan::MixFadeOut) is respelt in the same batch,
//   which also ends the HEAD duplicate definition of ?MixGainRampDown@@YAXPBFPBUSGainRamp@@PAJ@Z by the two twins.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E820, 139 bytes, `ret` (__cdecl).
// Module: libpk's software mixer: the per-block Process of the vtable 0x012BED90 effect (0x00C6DBA0, slot 2; ctor
//   0x00C6D9D0 landed) ramps two 4-byte gain records over a 128-sample block through MixGainRampUp 0x00C6DA40 /
//   MixGainRampDown 0x00C6DAD0; the vtable 0x012BEDB8 effect (0x00C6E8F0, slot 2; ctor 0x00C6E760) has byte-identical
//   private copies at 0x00C6E790 / 0x00C6E820 (separate TU).
// Layout: the SDK FilterPan from the shared header. __cdecl static members (callers pop with `add esp`).
// Callers: 0x00C6E924, 0x00C6E932, 0x00C6E958, 0x00C6E966.
// Callees: none.
// Shape: the 128-step loop unrolled by 4 with both pointers strength-reduced and biased by two elements (`add eax,8`,
//   `add edx,4`, offsets -8..+4 / -4..+2), stepped accumulator-first at the loop end (`add eax,10h ; add edx,8`).
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDERDISABLE_MIXFADE
#include "engine/CSalRenderDisable.h"
void CSalRenderDisable::MixFadeOut(const short* source, const FilterPan* filter, int* accumulator)
{
    unsigned short start = filter->gain;
    if (start == 0)
        return;
    short step = -filter->gainDelta;
    long gain = start;
    for (int i = 0; i < 128; ++i) {
        // The accumulator is indexed through its own copy of the loop counter: VC7.1 then steps the accumulator pointer
        // before the source pointer both at loop entry and at the loop end (retail 0x00C6E842 `add eax,8`, 0x00C6E848
        // `add edx,4`, 0x00C6E89D `add eax,10h ; add edx,8`); indexing both arrays by `i` steps them source-first at the end.
        const int sample = i;
        accumulator[sample] += (source[i] * gain) >> 15;
        gain += step;
    }
}