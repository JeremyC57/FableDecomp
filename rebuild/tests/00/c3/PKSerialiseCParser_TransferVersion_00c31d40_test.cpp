// Behaviour test for 00c31d40: PKSerialiseCParser::TransferVersion -- reading compares the stored dword with the version, writing sends it
#include <cstdio>
struct PKSerialiseCParser {
    void* m_pReader; void* m_pStream;
    unsigned int m_stored; bool m_fail; unsigned int m_written;
    bool TransferSize(unsigned int* size)
    {
        if (m_fail) return false;
        if (m_pReader) *size = m_stored; else m_written = *size;
        return true;
    }
    unsigned char TransferVersion(unsigned int version)
    {
        if (m_pReader) {
            unsigned int stored;
            if (!TransferSize(&stored)) return false;
            return stored == version;
        }
        return TransferSize(&version);
    }
};
int main()
{
    int reader = 0;
    PKSerialiseCParser p; p.m_pReader = &reader; p.m_pStream = 0; p.m_stored = 1; p.m_fail = false; p.m_written = 0;
    if (p.TransferVersion(1) != 1 || p.TransferVersion(2) != 0) { std::printf("00c31d40_TEST FAIL\n"); return 1; }
    p.m_fail = true;
    if (p.TransferVersion(1) != 0) { std::printf("00c31d40_TEST FAIL\n"); return 1; }
    p.m_fail = false; p.m_pReader = 0;
    if (p.TransferVersion(7) != 1 || p.m_written != 7) { std::printf("00c31d40_TEST FAIL\n"); return 1; }
    std::printf("00c31d40_TEST PASS\n");
    return 0;
}