// Behaviour test for 00c4aa30: PKCBufferedReader::GetSize -- returns the size word at +0x20
#include <cstdio>
class PKCBufferedReader {
public:
    virtual void v00() { }
    unsigned char* m_pBufferMemory; unsigned long m_blockSize, m_0c, m_lookBehind, m_bufferCount; void* m_pSource;
    unsigned char* m_pBufferStates; unsigned long m_size;
    unsigned long GetSize() { return m_size; }
};
int main()
{
    PKCBufferedReader r; r.m_size = 0x12345; r.m_blockSize = 7;
    int ok = r.GetSize() == 0x12345 && (char*)&r.m_size - (char*)&r == 0x20;
    std::printf(ok ? "00c4aa30_TEST PASS\n" : "00c4aa30_TEST FAIL\n");
    return ok ? 0 : 1;
}
