#include <cstdio>
#include <cstring>

// Model of CMemoryDataInputStream: a chunk buffer with a fast path and an overflow reader.
struct Stream {
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    const unsigned char* m_pOverflow;    // test-only: where the overflow reader takes its bytes
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        std::memcpy(buffer, m_pOverflow, size);
        m_pOverflow += size;
        m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned int)(m_position + size);
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
    void ReadValue(float& out) { float value; Read(&value, sizeof(value)); out = value; }
    void ReadValue(int& out) { int value; Read(&value, sizeof(value)); out = value; }
    void ReadValue(unsigned int& out) { unsigned int value; Read(&value, sizeof(value)); out = value; }
    void ReadValue(bool& out) { unsigned char value; Read(&value, sizeof(value)); out = value != 0; }
};

static unsigned char g_buf[64];
static unsigned char g_ovf[64];
static int g_len;
static int g_olen;
static void PutI(unsigned char* b, int& n, int v) { std::memcpy(b + n, &v, 4); n += 4; }
static void PutF(unsigned char* b, int& n, float v) { std::memcpy(b + n, &v, 4); n += 4; }
static void PutB(unsigned char* b, int& n, unsigned char v) { b[n++] = v; }

struct CReactionMatch_SecondaryAxisMRShifted {
    int ReactionType;
    int Axis;
    float LowerBoundOnAxis;
    float InvIntervalOnAxis;
    float MShiftZero;
    float MShiftWeight;
    float RShiftZero;
    float RShiftWeight;
    void TransferBinaryIn(Stream& stream)
    {
        stream.ReadValue(ReactionType);   // CReactionMatch::TransferBinaryIn 0x006FEFB0
        stream.ReadValue(Axis);
        stream.ReadValue(LowerBoundOnAxis);
        stream.ReadValue(InvIntervalOnAxis);
        stream.ReadValue(MShiftZero);
        stream.ReadValue(MShiftWeight);
        stream.ReadValue(RShiftZero);
        stream.ReadValue(RShiftWeight);
    }
};

static bool Run(int split)
{
    g_len = 0; g_olen = 0;
    if (0 < split) PutI(g_buf, g_len, 7); else PutI(g_ovf, g_olen, 7);
    if (1 < split) PutI(g_buf, g_len, 3); else PutI(g_ovf, g_olen, 3);
    if (2 < split) PutF(g_buf, g_len, 2.5f); else PutF(g_ovf, g_olen, 2.5f);
    if (3 < split) PutF(g_buf, g_len, 3.5f); else PutF(g_ovf, g_olen, 3.5f);
    if (4 < split) PutF(g_buf, g_len, 4.5f); else PutF(g_ovf, g_olen, 4.5f);
    if (5 < split) PutF(g_buf, g_len, 5.5f); else PutF(g_ovf, g_olen, 5.5f);
    if (6 < split) PutF(g_buf, g_len, 6.5f); else PutF(g_ovf, g_olen, 6.5f);
    if (7 < split) PutF(g_buf, g_len, 7.5f); else PutF(g_ovf, g_olen, 7.5f);
    CReactionMatch_SecondaryAxisMRShifted obj;
    std::memset(&obj, 0, sizeof(obj));
    Stream st;
    st.m_position = 100; st.m_pData = g_buf; st.m_remaining = g_len; st.m_pOverflow = g_ovf; st.overflowCalls = 0;
    obj.TransferBinaryIn(st);
    if (obj.ReactionType != 7 || obj.Axis != 3 || obj.LowerBoundOnAxis != 2.5f || obj.InvIntervalOnAxis != 3.5f || obj.MShiftZero != 4.5f || obj.MShiftWeight != 5.5f || obj.RShiftZero != 6.5f || obj.RShiftWeight != 7.5f)
        return false;
    if (st.m_position != 100u + (unsigned)(g_len + g_olen) || st.m_remaining != 0 || st.overflowCalls != 8 - split) return false;
    return true;
}

int main()
{
    for (int split = 0; split <= 8; ++split)
        if (!Run(split)) { std::printf("006ff040_TEST FAIL\n"); return 1; }
    std::printf("006ff040_TEST PASS\n");
    return 0;
}
