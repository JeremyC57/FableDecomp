// CWaveFile::ConvertToMono() -- average interleaved stereo 16-bit samples into a mono buffer
// Retail: Fable.exe @ 0x00AAD5D0, 116 bytes. PDB ?ConvertToMono@CWaveFile@@QAEXXZ
// Callees: CArray<short>::CArray(unsigned long) 0x00AADD80 (label "Vector_Construct_uint16_v2"),
// CArray<short>::operator=(const CArray<short>&) 0x00AADE80 (label "Vector_Assign_uint16"), the inlined
// CArray<short> destructor frees through 0x00BFEA14 (label "free"; it is the sized-delete/free import).
// CWaveFile (retail 24 bytes; PDB 28 with a 16-byte debug CArray): +0x00 Channels, +0x04 SampleLength,
// +0x08 SamplingFrequency, +0x0C CArray<short> AudioData (12-byte start/finish/end-of-storage vector).
// Audio/lip-sync layer: CWaveFile feeds CSpeechAnalyser / CLipSyncDataBank (replacement work later).
// Shape: only stereo files convert; AudioData and SampleLength are re-read every iteration (the short store can
// alias them), the mono buffer pointer is a register; sample = (L + R) >> 1 (arithmetic shift, no rounding).
// The mono buffer's start stays in edi through the assignment and the inlined destructor because the local does not
// escape: the constructor and operator= (both CArray<short> instantiations in retail's TU) are same-TU stand-ins.
// Leaf: ?ConvertToMono@CWaveFile@@QAEXXZ
// Name fix (NF195, 2026-11-20, q6206 / q6207): the local CArray<short> model and its same-TU stand-in constructor / operator= (over the
//   invented ConstructShorts_00aadd80 / AssignShorts_00aade80 helpers) are replaced by the shared engine/CWaveFile.h + engine/CArray.h
//   (CArray<short> is an STLport vector<short>): the size constructor and the assignment now bind STLport's
//   vector<short>::vector(size_type) 0x00AADD80 and vector<short>::operator= 0x00AADE80, declared only here
//   (FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE, FABLE_RETAIL_STLP_VECTOR_ASSIGN).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_C
#include "rebuild_abi.h"
#include "engine/CWaveFile.h"

void CWaveFile::ConvertToMono()
{
    if (Channels == 2)
    {
        CArray<short> mono(SampleLength);
        short* out = &mono[0];
        for (unsigned long i = 0; i < SampleLength; ++i)
            out[i] = (short)((AudioData[i * 2] + AudioData[i * 2 + 1]) >> 1);
        Channels = 1;
        AudioData = mono;
    }
}