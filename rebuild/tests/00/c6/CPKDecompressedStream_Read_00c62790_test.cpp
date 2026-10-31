// behaviour test for the parked 0x00C62790 CPKDecompressedStream::Read (copy of the logic over a stub decompressor)
#include <cstdio>
class C16BitSamplePtr { public:
    short* m_pData; long m_channels;
    void Advance(long frames) { m_pData += frames * m_channels; }
};
class CDecompressorInterface { public:
    long m_left, m_length, m_chunk, m_seeks; bool m_fail;
    long Seek(long, bool) { ++m_seeks; m_left = m_length; return m_fail ? -1 : 0; }
    long Decode(C16BitSamplePtr& d, long frames)
    {
        long n = frames < m_chunk ? frames : m_chunk; if (n > m_left) n = m_left;
        for (long i = 0; i < n * d.m_channels; ++i) d.m_pData[i] = 7;
        m_left -= n; return n;
    }
};
class CPKDecompressedStream { public:
    void* m_pSource; CDecompressorInterface* m_pDecompressor; long m_position; long m_userData; bool m_bFlag;
    long Read(const C16BitSamplePtr& destination, long frames);
};
long CPKDecompressedStream::Read(const C16BitSamplePtr& destination, long frames)
{
    long total = 0;
    if (!m_pDecompressor) return -1;
    if (m_position < 0) return -1;
    if (m_bFlag) return 0;
    C16BitSamplePtr cursor(destination);
    while (frames) {
        long read = m_pDecompressor->Decode(cursor, frames);
        if (read < 0) { total = -1; m_position = total; break; }
        if (read > 0) { m_position += read; total += read; frames -= read; cursor.Advance(read); }
        if (!frames) break;
        if (m_userData) {
            m_position = m_pDecompressor->Seek(0, true);
            if (m_position < 0) { total = -1; break; }
            if (m_userData > 0) --m_userData;
        } else { m_bFlag = true; break; }
    }
    return total;
}
int main()
{
    short buf[64] = { 0 };
    CDecompressorInterface d = { 5, 5, 100, 0, false };
    CPKDecompressedStream s = { 0, &d, 0, 1, false };
    C16BitSamplePtr p = { buf, 2 };
    long r = s.Read(p, 8);                 // 5 frames, rewind (1 loop left -> 0), 3 more
    if (r != 8 || d.m_seeks != 1 || s.m_userData != 0 || s.m_position != 3 || buf[15] != 7 || buf[16] != 0) { std::printf("00c62790_TEST FAIL\n"); return 1; }
    r = s.Read(p, 8);                      // 2 left, no loops: finished
    if (r != 2 || !s.m_bFlag) { std::printf("00c62790_TEST FAIL\n"); return 1; }
    if (s.Read(p, 8) != 0) { std::printf("00c62790_TEST FAIL\n"); return 1; }
    CPKDecompressedStream z = { 0, 0, 0, 0, false };
    if (z.Read(p, 1) != -1) { std::printf("00c62790_TEST FAIL\n"); return 1; }
    CDecompressorInterface f = { 1, 1, 4, 0, true };
    CPKDecompressedStream e = { 0, &f, 0, -1, false };
    if (e.Read(p, 4) != -1 || e.m_position != -1) { std::printf("00c62790_TEST FAIL\n"); return 1; }
    std::printf("00c62790_TEST PASS\n");
    return 0;
}