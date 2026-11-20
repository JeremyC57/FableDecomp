// Behaviour test for 0x00AAD5D0 CWaveFile::ConvertToMono() (NF195, 2026-11-20, q6206 / q6207: rewritten on the shared
// engine/CWaveFile.h + engine/CArray.h; links the row object, which carries the STLport vector<short>(size_type) and operator=
// bodies 0x00AADD80 / 0x00AADE80). Stereo samples are averaged pairwise into a mono buffer (arithmetic shift, no rounding);
// a mono file is left untouched.
#include <cstdio>
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_C
#include "rebuild_abi.h"
#include "engine/CWaveFile.h"
int main()
{
    static const short samples[8] = { 100, 200, -100, -301, 32767, 32767, -32768, -32768 };
    CWaveFile wave;
    for (int i = 0; i < 8; ++i) wave.AudioData.push_back(samples[i]);
    wave.Channels = 1; wave.SampleLength = 4;
    wave.ConvertToMono();
    bool ok = wave.Channels == 1 && wave.AudioData.size() == 8 && wave.AudioData[1] == 200;
    wave.Channels = 2;
    wave.ConvertToMono();
    ok = ok && wave.Channels == 1 && wave.AudioData.size() == 4 && wave.AudioData[0] == 150 && wave.AudioData[1] == -201
         && wave.AudioData[2] == 32767 && wave.AudioData[3] == -32768;
    std::printf(ok ? "00aad5d0_TEST PASS\n" : "00aad5d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
