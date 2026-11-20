// Behaviour test for 00c754a0: CStereoDelayLine scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class CDelayLine { public: CDelayLine() { } virtual ~CDelayLine() { ++g_base; } float* m_data; unsigned int m_size; unsigned int m_pos; };
class CStereoDelayLine { public: CStereoDelayLine() { } virtual ~CStereoDelayLine() { ++g_dtor; } CDelayLine m_lines[2]; };
int main()
{
    CStereoDelayLine* p = new CStereoDelayLine;
    delete p;
    int ok = g_dtor == 1 && g_base == 2;
    std::printf(ok ? "00c754a0_TEST PASS\n" : "00c754a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
