// Behaviour test for 00c75440: CStereoDelayLine::~CStereoDelayLine -- both lines detached, then destroyed last-to-first
#include <cstdio>
#include <cstring>
static char g_seq[8] = ""; static float* g_seenData[2]; static unsigned int g_seenSize[2]; static int g_n = 0;
class CDelayLine {
public:
    CDelayLine() : m_data(0), m_size(0), m_pos(0) { }
    virtual ~CDelayLine() { g_seenData[g_n] = m_data; g_seenSize[g_n] = m_size; ++g_n; std::strcat(g_seq, "D"); }
    void Detach() { m_data = 0; m_size = 0; }
    float* m_data; unsigned int m_size; unsigned int m_pos;
};
class CStereoDelayLine {
public:
    CStereoDelayLine() { }
    virtual ~CStereoDelayLine() { m_lines[0].Detach(); m_lines[1].Detach(); }
    CDelayLine m_lines[2];
};
int main()
{
    static float pool[64];
    CStereoDelayLine* s = new CStereoDelayLine;
    s->m_lines[0].m_data = pool; s->m_lines[0].m_size = 32; s->m_lines[1].m_data = pool + 32; s->m_lines[1].m_size = 32; s->m_lines[1].m_pos = 5;
    delete s;
    int ok = g_n == 2 && std::strcmp(g_seq, "DD") == 0 && g_seenData[0] == 0 && g_seenData[1] == 0 && g_seenSize[0] == 0 && g_seenSize[1] == 0
          && sizeof(CStereoDelayLine) == 0x24 && sizeof(CDelayLine) == 0x10;
    std::printf(ok ? "00c75440_TEST PASS\n" : "00c75440_TEST FAIL\n");
    return ok ? 0 : 1;
}
