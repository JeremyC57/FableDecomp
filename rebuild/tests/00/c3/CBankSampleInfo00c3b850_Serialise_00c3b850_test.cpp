// Behaviour test for 00c3b850: CBankSampleInfo::Serialise -- transfers every field in order and stops at the first failed transfer
#include <cstdio>
#include <string.h>

struct CString { const char* m_pData; };

struct CParser {
    int m_budget;                 // transfers allowed before failing
    int m_done;
    unsigned char m_log[64];
    int m_bytes;
    bool Transfer(const void* p, int size)
    {
        if (m_done >= m_budget) return false;
        ++m_done; memcpy(m_log + m_bytes, p, size); m_bytes += size; return true;
    }
    bool Serialise(unsigned long& v) { return Transfer(&v, 4); }
    bool Serialise(float& v) { return Transfer(&v, 4); }
    bool Serialise(unsigned short& v) { return Transfer(&v, 2); }
    bool Serialise(CString& s) { return Transfer(s.m_pData, 1); }
};

struct CBankSampleInfo {
    unsigned long m_id;
    CString m_name;
    unsigned long m_offset, m_length;
    unsigned short m_channels, m_bits;
    unsigned long m_rate;
    float m_volume, m_pitch;
    bool Serialise(CParser& parser)
    {
        return parser.Serialise(m_id) && parser.Serialise(m_name) && parser.Serialise(m_offset)
            && parser.Serialise(m_length) && parser.Serialise(m_channels) && parser.Serialise(m_bits)
            && parser.Serialise(m_rate) && parser.Serialise(m_volume) && parser.Serialise(m_pitch);
    }
};

static int fail() { std::printf("00c3b850_TEST FAIL\n"); return 1; }

int main()
{
    CBankSampleInfo info;
    info.m_id = 1; info.m_name.m_pData = "n"; info.m_offset = 2; info.m_length = 3;
    info.m_channels = 4; info.m_bits = 5; info.m_rate = 6; info.m_volume = 1.0f; info.m_pitch = 2.0f;
    CParser p; p.m_budget = 100; p.m_done = 0; p.m_bytes = 0;
    if (!info.Serialise(p) || p.m_done != 9 || p.m_bytes != 4 + 1 + 4 + 4 + 2 + 2 + 4 + 4 + 4) return fail();
    CParser q; q.m_budget = 5; q.m_done = 0; q.m_bytes = 0;
    if (info.Serialise(q) || q.m_done != 5) return fail();
    std::printf("00c3b850_TEST PASS\n");
    return 0;
}