// Behaviour test for 00c2d820: src/00c2d820.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAudioFormat::SamplesToBytes(unsigned long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAudioFormat::SamplesToBytes(unsigned long) -- converts a sample count to a byte count: whole blocks x block size
// Leaf: SamplesToBytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D820, 30 bytes (+2 int3 padding). Manifest label
// "CTCHeroExperience::AddSuccessfulBlockParry" is propagated from an unrelated body. Same audio-format record as
// BytesToSamples (0x00C2D800): word +0x0C = channel count, word +0x0E = codec (0 PCM, 1/2 Xbox ADPCM, 64 samples
// in a 36-byte block per channel). Callers 0x00C16370.., 0x00C2D955, 0x00C2E22A, 0x00C3468D.
// Callees: GetSamplesPerBlock() @ 0x00C2D790, GetBlockAlign() @ 0x00C2D7D0.
//
// Shape: unsigned `div` of the sample count by the samples per block, quotient kept in edx across the
// second call and multiplied by the block size (`imul eax,edx`).

struct CAudioFormat {
    unsigned long  m_length;          // +0x00
    unsigned long  m_loopStart;       // +0x04
    unsigned long  m_loopLength;      // +0x08
    unsigned short m_channels;        // +0x0C
    unsigned short m_codec;           // +0x0E 0 = PCM, 1/2 = ADPCM

    unsigned long GetSamplesPerBlock();   // 0x00C2D790
    unsigned long GetBlockAlign();        // 0x00C2D7D0
    unsigned long SamplesToBytes(unsigned long samples);
};

// Same-TU bodies of the two callees (retail's TU, above the caller; both byte-identical to 0x00C2D790 / 0x00C2D7D0):
// neither touches ecx or edx, so VC7.1 keeps `this` in ecx and the quotient in edx across the calls, as retail does.
__declspec(noinline) unsigned long CAudioFormat::GetSamplesPerBlock()
{
    switch (m_codec) {
    case 0: return m_channels;
    case 1: case 2: return m_channels * 64;
    }
    return 0;
}
__declspec(noinline) unsigned long CAudioFormat::GetBlockAlign()
{
    switch (m_codec) {
    case 0: return m_channels * 2;
    case 1: case 2: return m_channels * 36;
    }
    return 0;
}

unsigned long CAudioFormat::SamplesToBytes(unsigned long samples)
{
    unsigned long blocks = samples / GetSamplesPerBlock();
    return GetBlockAlign() * blocks;
}

// ---- stubs and driver ----
int main()
{
    CAudioFormat f; memset(&f, 0, sizeof(f));
    f.m_channels = 2; f.m_codec = 0;                       // PCM stereo: 2 samples per 4-byte block
    CHECK(f.SamplesToBytes(2000) == 4000 && f.SamplesToBytes(2001) == 4000);
    f.m_codec = 1;                                         // Xbox ADPCM stereo: 128 samples per 72-byte block
    CHECK(f.SamplesToBytes(1280) == 720 && f.SamplesToBytes(1280 + 127) == 720);
    f.m_codec = 2; f.m_channels = 1;
    CHECK(f.SamplesToBytes(192) == 108);
    if (fails == 0) printf("00c2d820_TEST PASS\n");
    return fails;
}
