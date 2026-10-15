// Behaviour test for 0x00994230 CDataOutputStream::WriteRepeatedByte: count copies of the byte arrive in chunks of at most 256.
#include <cstdio>
#include <cstring>
struct CDataOutputStream {
    unsigned char m_out[1024]; int m_len; int m_calls; int m_maxChunk;
    virtual void Write(const void* data, int size)
    { std::memcpy(m_out + m_len, data, size); m_len += size; ++m_calls; if (size > m_maxChunk) m_maxChunk = size; }
    virtual ~CDataOutputStream() {}
    void WriteRepeatedByte(unsigned char value, int count)
    {
        unsigned char buffer[256];
        std::memset(buffer, value, count < 256 ? count : 256);
        while (count > 0) {
            int size = count < 256 ? count : 256;
            Write(buffer, size);
            count -= size;
        }
    }
};
int main()
{
    CDataOutputStream s; s.m_len = 0; s.m_calls = 0; s.m_maxChunk = 0;
    s.WriteRepeatedByte(0xab, 600);
    if (s.m_len != 600 || s.m_calls != 3 || s.m_maxChunk != 256 || s.m_out[0] != 0xab || s.m_out[599] != 0xab) { std::printf("00994230_TEST FAIL\n"); return 1; }
    s.WriteRepeatedByte(1, 0);
    if (s.m_len != 600 || s.m_calls != 3) { std::printf("00994230_TEST FAIL\n"); return 1; }
    s.WriteRepeatedByte(2, 5);
    if (s.m_len != 605 || s.m_calls != 4 || s.m_out[604] != 2) { std::printf("00994230_TEST FAIL\n"); return 1; }
    std::printf("00994230_TEST PASS\n");
    return 0;
}
