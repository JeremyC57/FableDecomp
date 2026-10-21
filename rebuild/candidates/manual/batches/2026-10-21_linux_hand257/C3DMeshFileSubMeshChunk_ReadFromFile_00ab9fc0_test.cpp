// Behaviour test for the chunk reader at 0x00AB9FC0 (stream fast/slow paths, member order, bytes left for the base reader).
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
struct CSubMeshChunk : CBase {
    CCharString Name; int SubMeshIndex; int ParentIndex; int FirstChildIndex; int NextSiblingIndex;
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize)
    {
        unsigned long start = in.GetPosition();
        Name = in.ReadNullTerminatedString();
        SubMeshIndex = in.ReadLong(); ParentIndex = in.ReadLong(); FirstChildIndex = in.ReadLong(); NextSiblingIndex = in.ReadLong();
        unsigned long headerSize = in.GetPosition() - start;
        CBase::ReadFromFile(in, chunkSize - headerSize);
    }
};
int main()
{
    PutS("sub"); PutI(1); PutI(-1); PutI(2); PutI(3);
    CDataInputStream in; Open(in, 1000);
    CSubMeshChunk c; c.ReadFromFile(in, 50);
    if (std::strcmp(c.Name.text, "sub") != 0 || c.SubMeshIndex != 1 || c.ParentIndex != -1 || c.FirstChildIndex != 2 ||
        c.NextSiblingIndex != 3 || c.baseSize != 50 - 20 || in.overflowReads != 0) {
        std::printf("00ab9fc0_TEST FAIL\n"); return 1;
    }
    std::printf("00ab9fc0_TEST PASS\n");
    return 0;
}
