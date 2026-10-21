// Behaviour test for the chunk reader at 0x00ABA170 (stream fast/slow paths, member order, bytes left for the base reader).
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
struct CMatrix3x4 { float m[12]; };
static CMatrix3x4 ReadMatrix(CDataInputStream& in) { CMatrix3x4 m; for (int i = 0; i < 12; ++i) m.m[i] = in.ReadFloat(); return m; }
struct CBoneChunk : CBase {
    int BoneIndex; int ParentIndex; int FirstChildIndex; int NextSiblingIndex; CMatrix3x4 LocalizationMatrix; CCharString Name;
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize)
    {
        unsigned long start = in.GetPosition();
        BoneIndex = in.ReadLong(); ParentIndex = in.ReadLong(); FirstChildIndex = in.ReadLong(); NextSiblingIndex = in.ReadLong();
        LocalizationMatrix = ReadMatrix(in);
        if (in.GetPosition() - start < chunkSize)
            Name = in.ReadNullTerminatedString();
        unsigned long headerSize = in.GetPosition() - start;
        CBase::ReadFromFile(in, chunkSize - headerSize);
    }
};
int main()
{
    PutI(4); PutI(0); PutI(5); PutI(-1); for (int i = 0; i < 12; ++i) PutF((float)i); PutS("bone");
    CDataInputStream in; Open(in, 1000);
    CBoneChunk c; c.ReadFromFile(in, 80);
    CDataInputStream in2; Open(in2, 1000);
    CBoneChunk old; std::strcpy(old.Name.text, "keep"); old.ReadFromFile(in2, 64);   // a chunk that ends after the matrix
    if (c.BoneIndex != 4 || c.NextSiblingIndex != -1 || c.LocalizationMatrix.m[11] != 11.0f ||
        std::strcmp(c.Name.text, "bone") != 0 || c.baseSize != 80 - 69 ||
        std::strcmp(old.Name.text, "keep") != 0 || old.baseSize != 0) {
        std::printf("00aba170_TEST FAIL\n"); return 1;
    }
    std::printf("00aba170_TEST PASS\n");
    return 0;
}
