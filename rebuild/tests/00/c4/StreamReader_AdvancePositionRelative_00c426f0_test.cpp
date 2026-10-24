// behaviour test for 00c426f0: StreamReader::AdvancePositionRelative -- a forward move may reach but
// not pass the size, a backward move may reach but not pass zero; a refused move leaves the cursor.
#include <cstdio>
class StreamReader {
public:
    const unsigned char* m_pBuffer;
    unsigned int m_flags;
    unsigned int m_position;
    unsigned int m_size;
    bool AdvancePositionRelative(int offset)
    {
        if (offset >= 0) {
            offset += m_position;
            if ((unsigned int)offset > m_size)
                return false;
        } else {
            offset += m_position;
            if (offset < 0)
                return false;
        }
        m_position = offset;
        return true;
    }
};
int main()
{
    int ok = 1;
    StreamReader r;
    r.m_pBuffer = 0;
    r.m_flags = 0;
    r.m_position = 10;
    r.m_size = 100;
    if (!r.AdvancePositionRelative(20) || r.m_position != 30) ok = 0;
    if (!r.AdvancePositionRelative(70) || r.m_position != 100) ok = 0;
    if (r.AdvancePositionRelative(1) || r.m_position != 100) ok = 0;
    if (!r.AdvancePositionRelative(-100) || r.m_position != 0) ok = 0;
    if (r.AdvancePositionRelative(-1) || r.m_position != 0) ok = 0;
    if (!r.AdvancePositionRelative(0) || r.m_position != 0) ok = 0;
    if (!ok) { std::printf("00c426f0_TEST FAIL\n"); return 1; }
    std::printf("00c426f0_TEST PASS\n");
    return 0;
}