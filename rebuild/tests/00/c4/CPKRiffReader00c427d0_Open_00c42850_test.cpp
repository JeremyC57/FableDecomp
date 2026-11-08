// Behaviour test for CPKRiffReader::Open (0x00C42850): a RIFF/WAVE image with fmt, data and an optional one-loop smpl
// chunk opens with the channels, rate, data offset/size/pointer, duration in ms and loop bytes set and the position at the
// data; a bad RIFF/WAVE tag, an unsupported bit depth, a missing data chunk or two loops fail; no smpl chunk = no loop.
#include <cstdio>
#include <cstring>

typedef unsigned int u32; typedef unsigned short u16;
struct SRiffChunk { u32 m_id; u32 m_size; u32 m_offset; };
struct SRiffHeader { u32 Id; u32 Size; u32 Format; };
struct SChunkHeader { u32 m_id; u32 m_size; };
struct SSampleChunk { u32 f[7]; u32 SampleLoops; u32 SamplerData; };
struct SSampleLoop { u32 Id; u32 Type; u32 Start; u32 End; u32 Fraction; u32 PlayCount; };
struct SWaveFormat { u16 FormatTag; u16 Channels; u32 SamplesPerSec; u32 AvgBytesPerSec; u16 BlockAlign; u16 BitsPerSample; u16 Size; };

struct Reader {
    const unsigned char* m_pData; u32 m_position; u32 m_size;
    SWaveFormat m_format; u32 m_channels, m_sampleRate, m_dataSize, m_loopStart, m_loopEnd;
    const unsigned char* m_pWaveData; u32 m_dataOffset, m_durationMs;
    void BaseOpen(const void* d, u32 n) { m_pData = (const unsigned char*)d; m_position = 0; m_size = n; }
    bool Read(void* b, u32 n) { if (m_position + n > m_size) return false; std::memcpy(b, m_pData + m_position, n); m_position += n; return true; }
    bool Seek(u32 n) { if (m_position + n > m_size) return false; m_position += n; return true; }
    bool FindChunk(SRiffChunk* c, bool fromStart)
    {
        if (fromStart) m_position = 12;
        while (m_position < m_size) {
            SChunkHeader h; Read(&h, 8);
            c->m_size = h.m_size;
            if (c->m_id == h.m_id) { c->m_offset = m_position; return true; }
            if (c->m_size & 1) c->m_size++;
            if (!Seek(c->m_size)) return false;
        }
        return false;
    }
    bool Open(const void* data, u32 length)
    {
        BaseOpen(data, length);
        m_size = 12;
        SRiffHeader riff;
        if (!Read(&riff, 12)) return false;
        if (riff.Id != 0x46464952) return false;
        u32 size = riff.Size + 8;
        if (length == 0) m_size = size;
        else if (size > length) return false;
        else if (size < length) m_size = size;
        else m_size = length;
        if (riff.Format != 0x45564157) return false;
        u32 smplSize = 0;
        bool found = false;
        {
            SRiffChunk chunk;
            chunk.m_id = 0x20746d66;
            if (!FindChunk(&chunk, true)) return false;
            if (chunk.m_size < 0x10) return false;
            if (!Read(&m_format, 0x10)) return false;
            u16 bits = m_format.BitsPerSample, channels = m_format.Channels; u32 rate = m_format.SamplesPerSec;
            m_format.Size = 0;
            if (bits != 8 && bits != 4 && bits != 16) return false;
            m_channels = channels; m_sampleRate = rate;
            chunk.m_id = 0x61746164;
            if (!FindChunk(&chunk, true)) return false;
            m_dataOffset = chunk.m_offset; m_dataSize = chunk.m_size; m_pWaveData = m_pData + chunk.m_offset;
            m_durationMs = (u32)((long long)chunk.m_size * 1000 / (long long)m_format.AvgBytesPerSec);
            chunk.m_id = 0x6c706d73;
            if (FindChunk(&chunk, true)) { found = true; smplSize = chunk.m_size; }
        }
        if (found) {
            if (smplSize < 0x24) return false;
            SSampleChunk sample;
            if (!Read(&sample, 0x24)) return false;
            if (sample.SampleLoops != 0) {
                if (sample.SampleLoops > 1) return false;
                if (smplSize < 0x3c) return false;
                SSampleLoop loop;
                if (!Read(&loop, 0x18)) return false;
                m_loopStart = m_format.BlockAlign * loop.Start;
                m_loopEnd = m_format.BlockAlign * loop.End;
                if (m_loopStart > m_loopEnd || m_loopEnd >= m_dataSize) return false;
            } else { m_loopStart = 0; m_loopEnd = 0; }
        } else { m_loopStart = 0; m_loopEnd = 0; }
        m_position = m_dataOffset;
        return true;
    }
};

static unsigned char img[512]; static u32 len;
static void Put(const void* p, u32 n) { std::memcpy(img + len, p, n); len += n; }
static void Put32(u32 v) { Put(&v, 4); }
static void Build(u16 bits, u32 loops, bool smpl)
{
    len = 0; Put32(0x46464952); Put32(0); Put32(0x45564157);
    Put32(0x20746d66); Put32(16);
    SWaveFormat f = { 1, 2, 22050, 88200, 4, bits, 0 }; Put(&f, 16);
    Put32(0x61746164); Put32(400); for (int i = 0; i < 100; ++i) Put32(0);
    if (smpl) {
        Put32(0x6c706d73); Put32(loops ? 0x3c : 0x24);
        SSampleChunk s; std::memset(&s, 0, sizeof s); s.SampleLoops = loops; Put(&s, 0x24);
        if (loops) { SSampleLoop l = { 0, 0, 10, 50, 0, 0 }; Put(&l, 0x18); }
    }
    u32 riff = len - 8; std::memcpy(img + 4, &riff, 4);
}

int main()
{
    Reader r; bool ok = true;
    Build(16, 1, true);
    ok = ok && r.Open(img, len) && r.m_channels == 2 && r.m_sampleRate == 22050 && r.m_dataOffset == 44 && r.m_dataSize == 400
            && r.m_pWaveData == img + 44 && r.m_durationMs == 4 && r.m_loopStart == 40 && r.m_loopEnd == 200 && r.m_position == 44;
    Build(16, 0, false);
    ok = ok && r.Open(img, 0) && r.m_size == len && r.m_loopStart == 0 && r.m_loopEnd == 0;
    Build(16, 0, true);
    ok = ok && r.Open(img, len + 16) && r.m_size == len && r.m_loopEnd == 0;
    Build(16, 2, true);  ok = ok && !r.Open(img, len);
    Build(12, 0, false); ok = ok && !r.Open(img, len);
    Build(16, 0, false); ok = ok && !r.Open(img, len - 1);
    img[8] = 'X';        ok = ok && !r.Open(img, len);
    if (!ok) { std::printf("00c42850_TEST FAIL\n"); return 1; }
    std::printf("00c42850_TEST PASS\n");
    return 0;
}