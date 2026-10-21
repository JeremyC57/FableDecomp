// Behaviour test for the chunk reader at 0x00AAAB10 (stream fast/slow paths, member order, bytes left for the base reader).
#include <cstdio>
#include <cstring>
// Memory input stream model: position, read pointer and bytes left in the current chunk; the slow path
// (ReadWithSrcChunkOverflow) copies from the same buffer and counts its use.
struct CCharString { char text[64]; CCharString() { text[0] = 0; } };
struct CDataInputStream {
    unsigned long m_position;
    const unsigned char* m_pData;
    int m_remaining;
    int overflowReads;
    unsigned long GetPosition() { return m_position; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        std::memcpy(buffer, m_pData, size); m_pData += size; m_position += size; overflowReads++;
    }
    void Read(void* buffer, int size)
    {
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size; m_remaining -= size; m_position += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    int ReadLong() { int v; Read(&v, 4); return v; }
    unsigned int ReadULong() { unsigned int v; Read(&v, 4); return v; }
    float ReadFloat() { float v; Read(&v, 4); return v; }
    CCharString ReadNullTerminatedString()
    {
        CCharString s; int n = (int)std::strlen((const char*)m_pData) + 1;
        std::memcpy(s.text, m_pData, n); m_pData += n; m_position += n; m_remaining -= n; return s;
    }
};
struct CBase { unsigned long baseSize; CBase() : baseSize(0xFFFFFFFF) {} void ReadFromFile(CDataInputStream&, unsigned long size) { baseSize = size; } };
static unsigned char g_buf[512]; static int g_len;
static void PutI(int v) { std::memcpy(g_buf + g_len, &v, 4); g_len += 4; }
static void PutF(float v) { std::memcpy(g_buf + g_len, &v, 4); g_len += 4; }
static void PutS(const char* s) { int n = (int)std::strlen(s) + 1; std::memcpy(g_buf + g_len, s, n); g_len += n; }
static void Open(CDataInputStream& in, int remaining) { in.m_position = 100; in.m_pData = g_buf; in.m_remaining = remaining; in.overflowReads = 0; }
struct C3DVector { float X, Y, Z; };
struct CPlane { C3DVector Normal; float Distance; };
static C3DVector Read3DVector(CDataInputStream& in) { C3DVector v; v.X = in.ReadFloat(); v.Y = in.ReadFloat(); v.Z = in.ReadFloat(); return v; }
struct CPlaneArray {
    CPlane data[8]; unsigned long n;
    void resize(unsigned long size) { for (unsigned long i = n; i < size; ++i) { CPlane p = CPlane(); data[i] = p; } n = size; }
    CPlane& operator[](unsigned long i) { return data[i]; }
};
struct CConvexVolumeChunk {
    CCharString Name; CPlaneArray Planes;
    void ReadFromFile(CDataInputStream& in, unsigned long)
    {
        in.ReadULong();
        Name = in.ReadNullTerminatedString();
        unsigned long count = in.ReadULong();
        Planes.resize(count);
        for (unsigned long i = 0; i < count; ++i) {
            CPlane& plane = Planes[i];
            plane.Normal = Read3DVector(in);
            plane.Distance = in.ReadFloat();
        }
    }
};
int main()
{
    PutI(1); PutS("cv"); PutI(2);
    PutF(0.0f); PutF(1.0f); PutF(0.0f); PutF(5.0f);
    PutF(1.0f); PutF(0.0f); PutF(0.0f); PutF(-3.0f);
    CDataInputStream in; Open(in, 1000);
    CConvexVolumeChunk c; c.Planes.n = 5; c.ReadFromFile(in, 0);
    if (std::strcmp(c.Name.text, "cv") != 0 || c.Planes.n != 2 || c.Planes[0].Normal.Y != 1.0f || c.Planes[0].Distance != 5.0f ||
        c.Planes[1].Normal.X != 1.0f || c.Planes[1].Distance != -3.0f || in.m_position != 100 + 4 + 3 + 4 + 32) {
        std::printf("00aaab10_TEST FAIL\n"); return 1;
    }
    std::printf("00aaab10_TEST PASS\n");
    return 0;
}
