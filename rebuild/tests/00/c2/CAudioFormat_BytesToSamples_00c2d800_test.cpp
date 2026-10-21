// Behaviour test for 00c2d800: src/00c2d800.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAudioFormat::BytesToSamples(unsigned long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAudioFormat::BytesToSamples(unsigned long) -- converts a byte count to a sample count: whole blocks x samples per block
// Leaf: BytesToSamples
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D800, 30 bytes (+2 int3 padding). Manifest label
// "CTCHeroExperience::AddSuccessfulBlockParry" is propagated from an unrelated body. The object is a small
// audio-format record (word +0x0C = channel count, word +0x0E = codec: 0 PCM, 1/2 Xbox ADPCM); callers at
// 0x00C2D94B and 0x00C34640.. pair it with SamplesToBytes (0x00C2D820).
// Callees: GetBlockAlign() @ 0x00C2D7D0 (ADPCM: 36 bytes x channels; PCM: 2 x channels),
//          GetSamplesPerBlock() @ 0x00C2D790 (ADPCM: 64 x channels; PCM: channels).
//
// Shape: `div esi` after `xor edx,edx` = unsigned division of the byte count by the block size, the
// quotient moved to edx and multiplied by the samples per block after the second call (`imul eax,edx`).

struct CAudioFormat {
    unsigned long  m_length;          // +0x00
    unsigned long  m_loopStart;       // +0x04
    unsigned long  m_loopLength;      // +0x08
    unsigned short m_channels;        // +0x0C
    unsigned short m_codec;           // +0x0E 0 = PCM, 1/2 = ADPCM

    unsigned long GetSamplesPerBlock();   // 0x00C2D790
    unsigned long GetBlockAlign();        // 0x00C2D7D0
    unsigned long BytesToSamples(unsigned long bytes);
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

unsigned long CAudioFormat::BytesToSamples(unsigned long bytes)
{
    unsigned long blocks = bytes / GetBlockAlign();
    return GetSamplesPerBlock() * blocks;
}

// ---- stubs and driver ----
int main()
{
    CAudioFormat f; memset(&f, 0, sizeof(f));
    f.m_channels = 2; f.m_codec = 0;                       // PCM stereo: 4-byte blocks of 2 samples
    CHECK(f.BytesToSamples(4000) == 2000 && f.BytesToSamples(4003) == 2000);
    f.m_codec = 1;                                         // Xbox ADPCM stereo: 72-byte blocks of 128 samples
    CHECK(f.BytesToSamples(72 * 10) == 1280 && f.BytesToSamples(72 * 10 + 71) == 1280);
    f.m_codec = 2; f.m_channels = 1;
    CHECK(f.BytesToSamples(36 * 3) == 192);
    if (fails == 0) printf("00c2d800_TEST PASS\n");
    return fails;
}
