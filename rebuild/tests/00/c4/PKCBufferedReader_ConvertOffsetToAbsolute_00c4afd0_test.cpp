// Behaviour test for 0x00C4AFD0 PKCBufferedReader::ConvertOffsetToAbsolute: current (0) / end (1) / start (2) relative offsets become absolute and are sought; unknown origins and negative results fail without seeking
#include <cstdio>
enum ESeekOrigin { SEEK_ORIGIN_CURRENT = 0, SEEK_ORIGIN_END = 1, SEEK_ORIGIN_START = 2 };
struct PKCBufferedReader {
    unsigned long m_blockSize;
    unsigned long m_size;
    unsigned long m_firstBlock;
    unsigned long m_position;
    unsigned long m_lastSeek;
    int m_seeks;
    bool Seek(unsigned long offset) { ++m_seeks; m_lastSeek = offset; return offset <= m_size; }
    bool ConvertOffsetToAbsolute(long offset, ESeekOrigin origin);
};
bool PKCBufferedReader::ConvertOffsetToAbsolute(long offset, ESeekOrigin origin)
{
    long absolute;
    switch (origin) {
    case SEEK_ORIGIN_CURRENT:
        absolute = m_firstBlock * m_blockSize + m_position + offset;
        break;
    case SEEK_ORIGIN_END:
        absolute = m_size + offset;
        break;
    case SEEK_ORIGIN_START:
        absolute = offset;
        break;
    default:
        return false;
    }
    if (absolute < 0)
        return false;
    return Seek(absolute);
}
int main()
{
    PKCBufferedReader r;
    r.m_blockSize = 0x100; r.m_size = 0x1000; r.m_firstBlock = 3; r.m_position = 0x10; r.m_lastSeek = 0; r.m_seeks = 0;
    if (!r.ConvertOffsetToAbsolute(5, SEEK_ORIGIN_CURRENT) || r.m_lastSeek != 0x315) { std::printf("00c4afd0_TEST FAIL\n"); return 1; }
    if (!r.ConvertOffsetToAbsolute(-0x10, SEEK_ORIGIN_END) || r.m_lastSeek != 0xff0) { std::printf("00c4afd0_TEST FAIL\n"); return 1; }
    if (!r.ConvertOffsetToAbsolute(0x20, SEEK_ORIGIN_START) || r.m_lastSeek != 0x20) { std::printf("00c4afd0_TEST FAIL\n"); return 1; }
    if (r.ConvertOffsetToAbsolute(-1, SEEK_ORIGIN_START) || r.m_seeks != 3) { std::printf("00c4afd0_TEST FAIL\n"); return 1; }
    if (r.ConvertOffsetToAbsolute(0, (ESeekOrigin)3) || r.m_seeks != 3) { std::printf("00c4afd0_TEST FAIL\n"); return 1; }
    if (r.ConvertOffsetToAbsolute(0x10, SEEK_ORIGIN_END) || r.m_seeks != 4) { std::printf("00c4afd0_TEST FAIL\n"); return 1; }
    std::printf("00c4afd0_TEST PASS\n");
    return 0;
}