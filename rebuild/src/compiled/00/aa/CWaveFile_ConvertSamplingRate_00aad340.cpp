// CWaveFile::ConvertSamplingRate(unsigned long rate) -- linear-interpolation resample of the 16-bit sample array to a new rate
// Leaf: ?ConvertSamplingRate@CWaveFile@@QAEXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAD340, 584 bytes, `ret 4`. Ego_r.pdb ?ConvertSamplingRate@CWaveFile@@QAEXK@Z
//   (lib_wave_loader.obj index 8, between ConvertToMono 0x00AAD5D0's TU-mates); locals and scopes from Ego_d.pdb.
// Callees: CArray<short>(n) -> STLport vector<short>(n) 0x00AADD80, vector<short>::operator= 0x00AADE80, free 0x00BFEA14
//   (inlined ~vector).
// Retail-asm: 0x00AAD381 `fld [ebp-20h] ; fsub [ebp-24h] ; fistp dword ptr [ebp-18h]` (and 0x00AAD3C8, 0x00AAD42A:
//   GFFloatToLongFloor x3) and 0x00AAD4E1 `fld [ebp-2Ch] ; fistp dword ptr [ebp-30h]` (GFFloatToLongNear) -- shared helpers.
// Ego_d line table (C13 DEBUG_S_LINES carved from Ego_d.pdb): step 57, step_int 58, a 7-byte constant store 59, step_frac 60,
//   new_sample_length 61, new_data 62, pos_int / pos_frac 63 / 64 (7 B each), data_dest 65; the wrap block's `pos_int++` (83)
//   precedes `pos_frac -=` (84). Line 59 is FRACTION_BITS (the only other 7-byte initialiser, and line 60 uses it), so it is
//   declared after step_int (byte-neutral: RELOCATION_MATCH in both orders).
// Unreproduced: Ego_d lines 55 (328 B) and 69 (348 B) have no retail counterpart; not written.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_C
#include "fable_string.h"
#include "engine/CWaveFile.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"

void CWaveFile::ConvertSamplingRate(unsigned long rate)
{
    float step = (float)SamplingFrequency / (float)rate;
    unsigned long step_int = GFFloatToLongFloor(step);
    const unsigned long FRACTION_BITS = 16;
    unsigned long step_frac = GFFloatToLongFloor((step - (float)step_int) * (float)(1 << FRACTION_BITS));
    unsigned long new_sample_length = GFFloatToLongFloor((float)((double)SampleLength * (double)rate / (double)SamplingFrequency));
    CArray<short> new_data(Channels * new_sample_length);
    unsigned long pos_int = 0;
    unsigned long pos_frac = 0;
    short* data_dest = new_data.GetAsCArray();
    for (unsigned long c_sample = 0; c_sample < new_sample_length; c_sample++)
    {
        unsigned long pos_int_plus_1 = pos_int + 1;
        if (pos_int_plus_1 > SampleLength - 1)
            pos_int_plus_1 = SampleLength - 1;
        long weight1 = (1 << FRACTION_BITS) - pos_frac;
        long weight2 = pos_frac;
        for (unsigned long c_chan = 0; c_chan < Channels; c_chan++)
        {
            long sample1 = AudioData[pos_int * Channels + c_chan];
            long sample2 = AudioData[pos_int_plus_1 * Channels + c_chan];
            *data_dest++ = (short)GFFloatToLongNear((float)((sample1 * weight1 + sample2 * weight2) >> FRACTION_BITS));
        }
        pos_int += step_int;
        pos_frac += step_frac;
        if (pos_frac > (1 << FRACTION_BITS))
        {
            pos_int++;
            pos_frac -= (1 << FRACTION_BITS);
        }
    }
    SampleLength = new_sample_length;
    SamplingFrequency = rate;
    AudioData = new_data;
}