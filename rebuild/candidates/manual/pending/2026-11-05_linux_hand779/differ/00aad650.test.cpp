// Behaviour test for CWaveLoader::LoadWaveFile (0x00AAD650): a standalone model of the RIFF walk -- the file must start with
// "RIFF" <size> "WAVE"; chunks are walked by id + size (the position is restored to chunk start + size after each one);
// "fmt " fills the format header; "data" requires PCM (FormatTag 1) and converts the samples to signed 16-bit (8-bit samples
// are unsigned, shifted up by 128 and scaled by 256); the sample length is the sample count divided by the channel count.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
struct Wave { unsigned short channels; unsigned long frequency; std::vector<short> samples; unsigned long sampleLength; };
struct Reader { const unsigned char* p; unsigned long pos, len; };
static unsigned long U32(Reader& r) { unsigned long v = r.p[r.pos] | (r.p[r.pos+1] << 8) | (r.p[r.pos+2] << 16) | ((unsigned long)r.p[r.pos+3] << 24); r.pos += 4; return v; }
static unsigned short U16(Reader& r) { unsigned short v = (unsigned short)(r.p[r.pos] | (r.p[r.pos+1] << 8)); r.pos += 2; return v; }
static std::string Tag(Reader& r) { std::string s((const char*)r.p + r.pos, 4); r.pos += 4; return s; }
static bool Load(const std::vector<unsigned char>& file, Wave& out)
{
    Reader r = { &file[0], 0, (unsigned long)file.size() };
    if (Tag(r) != "RIFF") return false;
    unsigned long riffSize = U32(r); unsigned long riffEnd = r.pos + riffSize;
    if (Tag(r) != "WAVE") return false;
    unsigned short formatTag = 0, channels = 0, blockAlign = 0; unsigned long samplesPerSec = 0;
    bool got = false;
    while (r.pos < riffEnd) {
        std::string id = Tag(r); unsigned long size = U32(r); unsigned long start = r.pos;
        if (id == "fmt ") { formatTag = U16(r); channels = U16(r); samplesPerSec = U32(r); U32(r); blockAlign = U16(r); U16(r); }
        else if (id == "data") {
            if (formatTag != 1) return false;
            unsigned long count = size / blockAlign * channels;
            long bits = blockAlign * 8 / channels;
            std::vector<short> samples(count);
            if (bits == 16) for (unsigned long i = 0; i < count; ++i) samples[i] = (short)U16(r);
            else if (bits == 8) for (unsigned long i = 0; i < count; ++i) { samples[i] = (short)(((unsigned short)r.p[r.pos] - 0x80) << 8); ++r.pos; }
            else return false;
            out.channels = channels; out.frequency = samplesPerSec; out.samples = samples; out.sampleLength = count / channels; got = true;
        }
        r.pos = start + size;
    }
    return got;
}
static void Put32(std::vector<unsigned char>& f, unsigned long v) { for (int i = 0; i < 4; ++i) f.push_back((unsigned char)(v >> (8 * i))); }
static void Put16(std::vector<unsigned char>& f, unsigned short v) { f.push_back((unsigned char)v); f.push_back((unsigned char)(v >> 8)); }
static void PutTag(std::vector<unsigned char>& f, const char* t) { for (int i = 0; i < 4; ++i) f.push_back((unsigned char)t[i]); }
static std::vector<unsigned char> Make(unsigned short tag, unsigned short channels, unsigned short bits, const std::vector<unsigned char>& data, bool junk)
{
    std::vector<unsigned char> f; PutTag(f, "RIFF"); Put32(f, 0); PutTag(f, "WAVE");
    if (junk) { PutTag(f, "LIST"); Put32(f, 3); f.push_back(1); f.push_back(2); f.push_back(3); }
    PutTag(f, "fmt "); Put32(f, 16); Put16(f, tag); Put16(f, channels); Put32(f, 22050); Put32(f, 22050 * channels * bits / 8); Put16(f, (unsigned short)(channels * bits / 8)); Put16(f, bits);
    PutTag(f, "data"); Put32(f, (unsigned long)data.size()); f.insert(f.end(), data.begin(), data.end());
    unsigned long riff = (unsigned long)f.size() - 8; for (int i = 0; i < 4; ++i) f[8 + i - 4] = (unsigned char)(riff >> (8 * i));
    return f;
}
int main()
{
    int fail = 0; Wave w;
    std::vector<unsigned char> d16; Put16(d16, 0x1234); Put16(d16, 0xFFFE); Put16(d16, 7); Put16(d16, 8);
    if (!Load(Make(1, 2, 16, d16, true), w) || w.channels != 2 || w.frequency != 22050 || w.sampleLength != 2 || w.samples.size() != 4 || w.samples[0] != 0x1234 || w.samples[1] != -2 || w.samples[3] != 8) fail = 1;
    std::vector<unsigned char> d8; d8.push_back(0x80); d8.push_back(0xFF); d8.push_back(0x00);
    if (!Load(Make(1, 1, 8, d8, false), w) || w.channels != 1 || w.sampleLength != 3 || w.samples[0] != 0 || w.samples[1] != 0x7F00 || w.samples[2] != -32768) fail = 2;
    if (Load(Make(3, 1, 16, d16, false), w)) fail = 3;               // non-PCM data chunk is rejected
    if (Load(Make(1, 1, 24, d16, false), w)) fail = 4;               // 24-bit samples are rejected
    std::vector<unsigned char> bad = Make(1, 1, 16, d16, false); bad[0] = 'X';
    if (Load(bad, w)) fail = 5;                                       // not a RIFF file
    if (fail) { std::printf("00aad650_TEST FAIL %d\n", fail); return 1; }
    std::printf("00aad650_TEST PASS\n");
    return 0;
}
