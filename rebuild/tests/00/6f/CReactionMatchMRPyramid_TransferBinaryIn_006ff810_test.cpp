#include <cstdio>
#include <cstring>
// Behaviour model of CReactionMatch_MRPyramid::TransferBinaryIn @ 0x006FF810 (copy of the source's logic).
static int g_baseCalls = 0, g_overflowCalls = 0;
class CMemoryDataInputStream { public:
    void* m_pVtable;
    unsigned long m_position;
    long StreamSize;
    const unsigned char* m_pData;
    long SourceChunkStreamPos;
    long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++g_overflowCalls; float f = 9.5f; std::memcpy(buffer, &f, 4); m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned long)(m_position + size);
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
};
struct CReactionMatch_MRPyramid {
    void* __vftable;
    unsigned char _base[0x24];
    float ScarinessShift, AgreeablenessShift, AttractivenessShift;
    void BaseTransferBinaryIn(CMemoryDataInputStream&) { ++g_baseCalls; }
    void TransferBinaryIn(CMemoryDataInputStream& stream)
    {
        BaseTransferBinaryIn(stream);
        float value;
        stream.Read(&value, sizeof(value));
        ScarinessShift = value;
        stream.Read(&value, sizeof(value));
        AgreeablenessShift = value;
        stream.Read(&value, sizeof(value));
        AttractivenessShift = value;
    }
};
int main()
{
    float src[2] = { 1.25f, -2.5f };
    CMemoryDataInputStream s; std::memset(&s, 0, sizeof(s));
    s.m_pData = (const unsigned char*)src; s.m_remaining = 8; s.m_position = 100;
    CReactionMatch_MRPyramid m; std::memset(&m, 0, sizeof(m));
    m.TransferBinaryIn(s);
    int ok = g_baseCalls == 1 && m.ScarinessShift == 1.25f && m.AgreeablenessShift == -2.5f
          && m.AttractivenessShift == 9.5f && g_overflowCalls == 1 && s.m_remaining == 0 && s.m_position == 112;
    if (!ok) { std::printf("006ff810_TEST FAIL\n"); return 1; }
    std::printf("006ff810_TEST PASS\n");
    return 0;
}