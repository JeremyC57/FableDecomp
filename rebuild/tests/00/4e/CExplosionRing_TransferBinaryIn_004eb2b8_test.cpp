#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef long S32;
#else
typedef int S32;
#endif
// behaviour test: CExplosionRing::TransferBinaryIn reads NumExplosions, RingRadius, AngleOffset, SecondsBeforeSpawnNewRing in that order
static int g_overflowCalls = 0;
class CDataInputStream { public:
    void* m_pVtable;
    unsigned long m_position;
    long StreamSize;
    const unsigned char* m_pData;
    long SourceChunkStreamPos;
    long m_remaining;
    const unsigned char* m_pOverflow;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++g_overflowCalls;
        std::memcpy(buffer, m_pOverflow, size);
        m_pOverflow += size;
        m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    S32 ReadLong() { S32 v; Read(&v, 4); return v; }
    float ReadFloat() { float v; Read(&v, 4); return v; }
};
class CExplosionRing { public:
    float RingRadius;
    S32   NumExplosions;
    float AngleOffset;
    float SecondsBeforeSpawnNewRing;
    void TransferBinaryIn(CDataInputStream* stream);
};
void CExplosionRing::TransferBinaryIn(CDataInputStream* stream)
{
    NumExplosions = stream->ReadLong();
    RingRadius = stream->ReadFloat();
    AngleOffset = stream->ReadFloat();
    SecondsBeforeSpawnNewRing = stream->ReadFloat();
}
int main()
{
    unsigned char chunk[8];
    unsigned char over[8];
    int n = 6; float r = 12.5f, a = 0.25f, s = 3.0f;
    std::memcpy(chunk, &n, 4); std::memcpy(chunk + 4, &r, 4);
    std::memcpy(over, &a, 4); std::memcpy(over + 4, &s, 4);
    CDataInputStream in;
    in.m_pVtable = 0; in.m_position = 0; in.StreamSize = 0; in.m_pData = chunk; in.SourceChunkStreamPos = 0; in.m_remaining = 8; in.m_pOverflow = over;
    CExplosionRing ring;
    ring.RingRadius = 0; ring.NumExplosions = 0; ring.AngleOffset = 0; ring.SecondsBeforeSpawnNewRing = 0;
    ring.TransferBinaryIn(&in);
    if (ring.NumExplosions != 6 || ring.RingRadius != 12.5f || ring.AngleOffset != 0.25f
        || ring.SecondsBeforeSpawnNewRing != 3.0f || g_overflowCalls != 2 || in.m_position != 16) {
        std::printf("004eb2b8_TEST FAIL\n"); return 1;
    }
    std::printf("004eb2b8_TEST PASS\n");
    return 0;
}