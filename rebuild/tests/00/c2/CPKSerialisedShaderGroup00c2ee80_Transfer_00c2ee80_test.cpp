// Behaviour test for 00c2ee80 CPKSerialisedShaderGroup_00c2ee80::Transfer (copy of the source model; the
// parser version check and the three sub-record transfers are spies that log their call order).
#include <cstdio>

static int g_log[8];
static int g_n = 0;
static bool g_result[4];

struct PKSerialiseCParser {
    void* m_pReader;
    void* m_pStream;
    bool TransferVersion(unsigned long version) { g_log[g_n++] = (int)(version * 10); return g_result[0]; }
};

struct CPKSubRecordA { unsigned char _body[0x10]; bool Transfer(PKSerialiseCParser&) { g_log[g_n++] = 1; return g_result[1]; } };
struct CPKSubRecordB { unsigned char _body[0x10]; bool Transfer(PKSerialiseCParser&) { g_log[g_n++] = 2; return g_result[2]; } };
struct CPKSubRecordC { unsigned char _body[0x10]; bool Transfer(PKSerialiseCParser&) { g_log[g_n++] = 3; return g_result[3]; } };

struct CPKSerialisedShaderGroup_00c2ee80 {
    CPKSubRecordA m_first;
    CPKSubRecordB m_second;
    CPKSubRecordC m_third;
    int Transfer(PKSerialiseCParser& parser);
};

int CPKSerialisedShaderGroup_00c2ee80::Transfer(PKSerialiseCParser& parser)
{
    if (parser.TransferVersion(1))
        if (m_first.Transfer(parser))
            if (m_second.Transfer(parser))
                if (m_third.Transfer(parser))
                    return 1;
    return 0;
}

static int Run(bool a, bool b, bool c, bool d, int expectRet, int expectCalls)
{
    g_result[0] = a; g_result[1] = b; g_result[2] = c; g_result[3] = d; g_n = 0;
    CPKSerialisedShaderGroup_00c2ee80 rec;
    PKSerialiseCParser p; p.m_pReader = 0; p.m_pStream = 0;
    int r = rec.Transfer(p);
    if (r != expectRet || g_n != expectCalls) return 0;
    static const int order[4] = { 10, 1, 2, 3 };
    for (int i = 0; i < g_n; ++i) if (g_log[i] != order[i]) return 0;
    return 1;
}

int main()
{
    int ok = Run(true, true, true, true, 1, 4)
          && Run(false, true, true, true, 0, 1)
          && Run(true, false, true, true, 0, 2)
          && Run(true, true, false, true, 0, 3)
          && Run(true, true, true, false, 0, 4);
    if (!ok) { std::printf("00c2ee80_TEST FAIL\n"); return 1; }
    std::printf("00c2ee80_TEST PASS\n");
    return 0;
}