#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_log[64]; static int g_n = 0;
struct CDataOutputStream {
    void WriteULong(unsigned long v) { g_n += std::sprintf(g_log + g_n, "U%lu;", v); }
    void WriteBool(bool v) { g_n += std::sprintf(g_log + g_n, "B%d;", v ? 1 : 0); }
    void WriteByte(unsigned char v) { g_n += std::sprintf(g_log + g_n, "C%u;", (unsigned)v); }
};
struct CBaseBlock {
    unsigned long m_id; unsigned long m_size; bool m_flagA; bool m_flagB; unsigned char m_type;
    void SaveBinary(CDataOutputStream& s)
    {
        s.WriteULong(m_id); s.WriteULong(m_size); s.WriteBool(m_flagA); s.WriteByte(m_type); s.WriteBool(m_flagB);
    }
};
static bool Check()
{
    CBaseBlock b; b.m_id = 7; b.m_size = 4096; b.m_flagA = true; b.m_flagB = false; b.m_type = 200;
    CDataOutputStream s; b.SaveBinary(s);
    return std::strcmp(g_log, "U7;U4096;B1;C200;B0;") == 0;
}
int main()
{
    if (!Check()) { std::printf("00a8e6c0_TEST FAIL\n"); return 1; }
    std::printf("00a8e6c0_TEST PASS\n");
    return 0;
}
