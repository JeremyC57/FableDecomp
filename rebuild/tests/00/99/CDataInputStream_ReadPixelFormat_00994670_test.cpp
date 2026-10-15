// Behaviour test for 0x00994670 CDataInputStream::ReadPixelFormat: 6 bytes from the chunk (or the overflow reader, or zeros
// when out of range) feed CPixelFormat::Initialise, whose result is returned.
#include <cstdio>
#include <cstring>
struct CPixelFormatInit { unsigned char m_bytes[6]; };
struct CPixelFormat {
    unsigned int m_format;
    CPixelFormat() {}
    void Initialise(const CPixelFormatInit& init) { m_format = init.m_bytes[0] | (init.m_bytes[5] << 8); }
};
static int g_overflow = 0;
struct CStream {
    unsigned int StreamPos; const unsigned char* CurrentSourcePtr; int SourceChunkBytesRemaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++g_overflow; std::memset(buffer, 7, size); }
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        long long np = (long long)(unsigned int)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    CPixelFormat ReadPixelFormat()
    {
        CPixelFormatInit init; std::memset(&init, 0, 6);
        Read(&init, 6);
        CPixelFormat format; format.Initialise(init);
        return format;
    }
};
int main()
{
    unsigned char data[6] = { 3, 0, 0, 0, 0, 9 };
    CStream s; s.StreamPos = 0; s.CurrentSourcePtr = data; s.SourceChunkBytesRemaining = 6;
    if (s.ReadPixelFormat().m_format != 0x903 || s.StreamPos != 6 || s.SourceChunkBytesRemaining != 0) { std::printf("00994670_TEST FAIL\n"); return 1; }
    if (s.ReadPixelFormat().m_format != 0x707 || g_overflow != 1) { std::printf("00994670_TEST FAIL\n"); return 1; }
    s.StreamPos = 0x7ffffffc;
    if (s.ReadPixelFormat().m_format != 0 || g_overflow != 1) { std::printf("00994670_TEST FAIL\n"); return 1; }
    std::printf("00994670_TEST PASS\n");
    return 0;
}