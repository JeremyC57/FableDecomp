// Behaviour test for HasWaveBankSegments (0x00C30E90): true only when both wave-bank segments open.
#include <cstdio>
#include <cstring>
struct CSegmentFile { int ok_wave, ok_table, calls; long OpenSegment(const char* name)
{ calls++; if (!std::strcmp(name, "LHAudioWaveData")) return ok_wave ? 0 : -1; return ok_table ? 0 : -1; } };
static bool HasWaveBankSegments(CSegmentFile* file)
{
    bool failed = file->OpenSegment("LHAudioWaveData") < 0 || file->OpenSegment("LHAudioBankSampleTable") < 0;
    return !failed;
}
int main()
{
    CSegmentFile a = { 1, 1, 0 }, b = { 0, 1, 0 }, c = { 1, 0, 0 };
    if (!HasWaveBankSegments(&a) || a.calls != 2 || HasWaveBankSegments(&b) || b.calls != 1 || HasWaveBankSegments(&c))
    { std::printf("00c30e90_TEST FAIL\n"); return 1; }
    std::printf("00c30e90_TEST PASS\n"); return 0;
}