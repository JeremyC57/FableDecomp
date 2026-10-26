// Behaviour test for the chunk reader at 0x00AA9BA0 (stream fast/slow paths, member order, bytes left for the base reader).
#include <cstdio>
#include <cstring>
// Memory input stream model: position, read pointer and bytes left in the current chunk; the slow path
// (ReadWithSrcChunkOverflow) copies from the same buffer and counts its use.
class CCharString { public: char text[64]; CCharString() { text[0] = 0; } };
class CDataInputStream { public:
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
class C3DVector { public: float X, Y, Z; };
struct CHelperPointChunk : CBase {
    C3DVector Position; int SubMeshIndex; int BoneIndex; CCharString Name;
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize)
    {
        unsigned long start = in.GetPosition();
        Position.X = in.ReadFloat(); Position.Y = in.ReadFloat(); Position.Z = in.ReadFloat();
        SubMeshIndex = in.ReadLong(); BoneIndex = in.ReadLong();
        Name = in.ReadNullTerminatedString();
        unsigned long headerSize = in.GetPosition() - start;
        CBase::ReadFromFile(in, chunkSize - headerSize);
    }
};
int main()
{
    PutF(1.0f); PutF(2.0f); PutF(3.0f); PutI(7); PutI(-1); PutS("hp_a");
    CDataInputStream in; Open(in, 12);   // the 4th read takes the overflow path
    CHelperPointChunk c; c.ReadFromFile(in, 100);
    if (c.Position.X != 1.0f || c.Position.Z != 3.0f || c.SubMeshIndex != 7 || c.BoneIndex != -1 ||
        std::strcmp(c.Name.text, "hp_a") != 0 || c.baseSize != 100 - 25 || in.overflowReads != 2) {
        std::printf("00aa9ba0_TEST FAIL\n"); return 1;
    }
    std::printf("00aa9ba0_TEST PASS\n");
    return 0;
}