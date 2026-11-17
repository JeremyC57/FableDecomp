// Behaviour test for GFSerialiseVectorBinaryIn<CCharString> (0x00433273): the 4-byte count comes from the chunk when it holds
// four bytes (advancing chunk, length and position) or from the overflow reader otherwise; the vector is resized to the count
// and each element is assigned the next null-terminated string.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    CCharString() { text[0] = 0; }
    CCharString(const char* s) { std::strcpy(text, s); }
    char text[32];
};
class CMemoryDataInputStream {
public:
    CMemoryDataInputStream(const char* data, long chunkLen, const char* const* strings)
        : position(0), chunkBase((long)0), chunkLength(chunkLen), m_data(data), m_strings(strings), m_next(0), overflowReads(0) {}
    virtual ~CMemoryDataInputStream() {}
    virtual CCharString ReadNullTerminatedString() { return CCharString(m_strings[m_next++]); }
    long position; long chunkBase; long chunkLength;
    const char* m_data; const char* const* m_strings; int m_next; int overflowReads;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { std::memcpy(buffer, m_data + position, size); position += size; ++overflowReads; }
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition)) return;
        if (chunkLength >= size) {
            std::memcpy(buffer, m_data + chunkBase, size);
            chunkBase += size; chunkLength -= size; position += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
class StringVector {
public:
    StringVector() : count(0), resizes(0) {}
    void resize(unsigned int n) { count = n; for (unsigned int i = 0; i < n; ++i) items[i] = CCharString(); ++resizes; }
    CCharString& operator[](unsigned int n) { return items[n]; }
    CCharString items[8]; unsigned int count; int resizes;
};
void GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, StringVector& out)
{
    CCharString defaultVal;
    int count;                                           // the stream's 32-bit count (retail `long`)
    stream.Read(&count, 4);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        out[i] = stream.ReadNullTerminatedString();
}
int main()
{
    int bad = 0;
    const char* strings[3] = { "HERO", "GUILD", "BOWERSTONE" };
    int three = 3;
    char data[8]; std::memcpy(data, &three, 4);
    CMemoryDataInputStream chunked(data, 8, strings);
    StringVector v;
    GFSerialiseVectorBinaryIn(chunked, v);
    if (v.count != 3 || v.resizes != 1 || std::strcmp(v[0].text, "HERO") || std::strcmp(v[2].text, "BOWERSTONE")) bad = 1;
    if (chunked.position != 4 || chunked.chunkBase != 4 || chunked.chunkLength != 4 || chunked.overflowReads != 0) bad = 1;
    int two = 2; std::memcpy(data, &two, 4);
    CMemoryDataInputStream overflow(data, 2, strings);
    StringVector w;
    GFSerialiseVectorBinaryIn(overflow, w);
    if (w.count != 2 || overflow.overflowReads != 1 || std::strcmp(w[1].text, "GUILD")) bad = 1;
    if (bad) { std::printf("00433273_TEST FAIL\n"); return 1; }
    std::printf("00433273_TEST PASS\n");
    return 0;
}