// Behaviour test for CAnimComponentStrikeResponseAnim::TransferBinaryIn (0x005DBF50): length prefix
// read through the inlined stream Read (fast chunk path / overflow path), then the name read only
// when the length is non-zero.
#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[32];
    CCharString() { text[0] = 0; }
};

class CDataInputStream { public:
    unsigned int m_position;
    const unsigned char* m_pData;
    long m_remaining;
    int overflowCalls;
    unsigned int overflowValue;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        std::memcpy(buffer, &overflowValue, size);
        m_position += size;
    }
    CCharString ReadString(unsigned int length)
    {
        CCharString s;
        std::memcpy(s.text, m_pData, length);
        s.text[length] = 0;
        m_pData += length;
        m_remaining -= length;
        m_position += length;
        return s;
    }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned int)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CAnimComponentStrikeResponseAnim { public:
    CCharString ResponseAnimName;
    void TransferBinaryIn(CDataInputStream& in)
    {
        unsigned int value;
        in.Read(&value, sizeof(value));
        unsigned int length = value;
        if (length > 0)
            ResponseAnimName = in.ReadString(length);
    }
};

int main()
{
    unsigned char buf[16] = { 5, 0, 0, 0, 'S', 'l', 'a', 's', 'h', 0 };
    CDataInputStream in = { 0, buf, 16, 0, 0 };
    CAnimComponentStrikeResponseAnim c;
    std::strcpy(c.ResponseAnimName.text, "old");
    c.TransferBinaryIn(in);
    if (std::strcmp(c.ResponseAnimName.text, "Slash") != 0 || in.m_position != 9 || in.overflowCalls != 0) {
        std::printf("005dbf50_TEST FAIL\n"); return 1;
    }
    unsigned char zero[4] = { 0, 0, 0, 0 };
    CDataInputStream in2 = { 0, zero, 4, 0, 0 };
    c.TransferBinaryIn(in2);
    if (std::strcmp(c.ResponseAnimName.text, "Slash") != 0 || in2.m_position != 4) {
        std::printf("005dbf50_TEST FAIL\n"); return 1;
    }
    // chunk too short: the overflow reader supplies a zero length -> name untouched
    CDataInputStream in3 = { 0, zero, 2, 0, 0 };
    c.TransferBinaryIn(in3);
    if (in3.overflowCalls != 1 || std::strcmp(c.ResponseAnimName.text, "Slash") != 0) {
        std::printf("005dbf50_TEST FAIL\n"); return 1;
    }
    std::printf("005dbf50_TEST PASS\n");
    return 0;
}