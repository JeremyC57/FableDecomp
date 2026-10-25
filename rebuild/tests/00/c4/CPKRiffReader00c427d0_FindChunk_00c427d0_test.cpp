// Behaviour test for 00c427d0: CPKRiffReader::FindChunk -- walks the RIFF sub-chunk headers (optionally from offset 12) to the chunk with the wanted id, recording its size and data offset; odd sizes are padded when skipping
#include <cstdio>
#include <string.h>

struct SRiffChunk { unsigned int m_id, m_size, m_offset; };
struct SRiffHeader { unsigned int m_id, m_size; };

class CPKRiffReader {
public:
    const unsigned char* m_pData;
    unsigned long m_position;
    unsigned long m_size;
    bool Read(void* pDest, unsigned long bytes) { memcpy(pDest, m_pData + m_position, bytes); m_position += bytes; return true; }
    bool Seek(long delta)
    {
        long to = (long)m_position + delta;
        if (to < 0 || (unsigned long)to > m_size) return false;
        m_position = (unsigned long)to;
        return true;
    }
    bool FindChunk(SRiffChunk* pChunk, bool fromStart)
    {
        if (fromStart)
            m_position = 12;
        while (m_position < m_size) {
            SRiffHeader header;
            Read(&header, 8);
            pChunk->m_size = header.m_size;
            if (pChunk->m_id == header.m_id) {
                pChunk->m_offset = m_position;
                return true;
            }
            if (pChunk->m_size & 1)
                pChunk->m_size++;
            if (!Seek(pChunk->m_size))
                return false;
        }
        return false;
    }
};

static void put(unsigned char* p, unsigned long v) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); p[2] = (unsigned char)(v >> 16); p[3] = (unsigned char)(v >> 24); }
static int fail() { std::printf("00c427d0_TEST FAIL\n"); return 1; }

int main()
{
    unsigned char file[48];
    memset(file, 0, sizeof(file));
    put(file + 12, 0x11); put(file + 16, 3);          // chunk A, 3 bytes (+1 pad) at 20
    put(file + 24, 0x22); put(file + 28, 4);          // chunk B, 4 bytes at 32
    put(file + 36, 0x33); put(file + 40, 2);          // chunk C, 2 bytes at 44
    CPKRiffReader r; r.m_pData = file; r.m_position = 0; r.m_size = 46;
    SRiffChunk c = { 0x22, 0, 0 };
    if (!r.FindChunk(&c, true) || c.m_size != 4 || c.m_offset != 32) return fail();
    SRiffChunk d = { 0x33, 0, 0 };
    r.m_position = 36;
    if (!r.FindChunk(&d, false) || d.m_offset != 44 || d.m_size != 2) return fail();
    SRiffChunk e = { 0x44, 0, 0 };
    if (r.FindChunk(&e, true)) return fail();
    std::printf("00c427d0_TEST PASS\n");
    return 0;
}