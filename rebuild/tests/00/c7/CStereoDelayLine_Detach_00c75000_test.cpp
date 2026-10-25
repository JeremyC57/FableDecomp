// Behaviour test for 00c75000: CStereoDelayLine::Detach -- both lines' data and size cleared, positions kept
#include <cstdio>
class CDelayLine {
public:
    CDelayLine() : m_data(0), m_size(0), m_pos(0) { }
    virtual ~CDelayLine() { }
    void Detach() { m_data = 0; m_size = 0; }
    float* m_data; unsigned int m_size; unsigned int m_pos;
};
class CStereoDelayLine {
public:
    CStereoDelayLine() { }
    virtual ~CStereoDelayLine() { }
    void Detach() { m_lines[0].Detach(); m_lines[1].Detach(); }
    CDelayLine m_lines[2];
};
int main()
{
    static float pool[64];
    CStereoDelayLine s;
    s.m_lines[0].m_data = pool; s.m_lines[0].m_size = 32; s.m_lines[0].m_pos = 3;
    s.m_lines[1].m_data = pool + 32; s.m_lines[1].m_size = 32; s.m_lines[1].m_pos = 5;
    s.Detach();
    int ok = s.m_lines[0].m_data == 0 && s.m_lines[0].m_size == 0 && s.m_lines[1].m_data == 0 && s.m_lines[1].m_size == 0
          && s.m_lines[0].m_pos == 3 && s.m_lines[1].m_pos == 5 && sizeof(CStereoDelayLine) == 0x24;
    std::printf(ok ? "00c75000_TEST PASS\n" : "00c75000_TEST FAIL\n");
    return ok ? 0 : 1;
}
