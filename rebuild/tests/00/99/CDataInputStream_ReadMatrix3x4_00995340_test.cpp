// Behaviour test for 0x00995340 CDataInputStream::ReadMatrix3x4: twelve floats read in order through the chunk fast
// path, falling back to the overflow reader when the chunk runs out.
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { C3DVector XAxis, YAxis, ZAxis, Position; };

static float g_over[16];
static int g_overUsed = 0;

struct CDataInputStream {
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        std::memcpy(buffer, &g_over[g_overUsed], size);
        ++g_overUsed;
        m_position += size;
    }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(m_position + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff))
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
    float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    CMatrix3x4 ReadMatrix3x4()
    {
        CMatrix3x4 m;
        m.XAxis.X = ReadFloat(); m.XAxis.Y = ReadFloat(); m.XAxis.Z = ReadFloat();
        m.YAxis.X = ReadFloat(); m.YAxis.Y = ReadFloat(); m.YAxis.Z = ReadFloat();
        m.ZAxis.X = ReadFloat(); m.ZAxis.Y = ReadFloat(); m.ZAxis.Z = ReadFloat();
        m.Position.X = ReadFloat(); m.Position.Y = ReadFloat(); m.Position.Z = ReadFloat();
        return m;
    }
};

int main()
{
    float chunk[8];
    int i;
    for (i = 0; i < 8; ++i) chunk[i] = (float)(i + 1);
    for (i = 0; i < 16; ++i) g_over[i] = (float)(100 + i);
    CDataInputStream s;
    s.m_position = 0;
    s.m_pData = (const unsigned char*)chunk;
    s.m_remaining = 8 * 4;
    CMatrix3x4 m = s.ReadMatrix3x4();
    int ok = 1;
    if (m.XAxis.X != 1.0f || m.XAxis.Z != 3.0f || m.YAxis.X != 4.0f || m.ZAxis.Y != 8.0f) ok = 0;
    if (m.ZAxis.Z != 100.0f || m.Position.X != 101.0f || m.Position.Z != 103.0f) ok = 0;
    if (g_overUsed != 4 || s.m_position != 48 || s.m_remaining != 0) ok = 0;
    if (ok) std::printf("00995340_TEST PASS\n"); else std::printf("00995340_TEST FAIL\n");
    return ok ? 0 : 1;
}