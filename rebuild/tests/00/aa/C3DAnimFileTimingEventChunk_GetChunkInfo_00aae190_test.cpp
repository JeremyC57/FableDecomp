#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_live = 0;
struct CCharString {
    char* m_pText;
    CCharString(const char* text, long length)
    {
        size_t n = length < 0 ? std::strlen(text) : (size_t)length;
        m_pText = (char*)std::malloc(n + 1); std::memcpy(m_pText, text, n); m_pText[n] = 0; ++g_live;
    }
    CCharString(const CCharString& o) { m_pText = (char*)std::malloc(std::strlen(o.m_pText) + 1); std::strcpy(m_pText, o.m_pText); ++g_live; }
    ~CCharString() { std::free(m_pText); --g_live; }
    static CCharString FormatDouble(long digits, double value)
    {
        char b[64]; std::sprintf(b, "%.*f", (int)digits, value); return CCharString(b, -1);
    }
};
CCharString operator+(const CCharString& left, const CCharString& right)
{
    char* b = (char*)std::malloc(std::strlen(left.m_pText) + std::strlen(right.m_pText) + 1);
    std::strcpy(b, left.m_pText); std::strcat(b, right.m_pText);
    CCharString r(b, -1); std::free(b); return r;
}
CCharString operator+(const CCharString& left, const char* right) { return left + CCharString(right, -1); }
struct C3DAnimFileTimingEventChunk {
    CCharString Name;
    float Time;
    C3DAnimFileTimingEventChunk() : Name("Strike", -1), Time(1.5f) {}
    CCharString GetChunkInfo() const;
};
CCharString C3DAnimFileTimingEventChunk::GetChunkInfo() const
{
    return CCharString("Name: ", -1) + Name + ", Time: " + CCharString::FormatDouble(6, Time);
}
int main()
{
    int ok;
    {
        C3DAnimFileTimingEventChunk c;
        CCharString s = c.GetChunkInfo();
        ok = std::strcmp(s.m_pText, "Name: Strike, Time: 1.500000") == 0 && g_live == 2;
    }
    if (!ok || g_live != 0) { std::printf("00aae190_TEST FAIL\n"); return 1; }
    std::printf("00aae190_TEST PASS\n");
    return 0;
}