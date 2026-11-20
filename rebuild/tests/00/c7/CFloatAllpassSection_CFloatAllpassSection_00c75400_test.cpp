#include <cstdio>
#include <new>
// CDelayLine::~CDelayLine: detach the pooled sample memory (data and size cleared, position kept).
static int g_dtors = 0;
class CDelayLine {
public:
    CDelayLine() : m_data(0), m_size(0), m_pos(0) {}
    virtual ~CDelayLine() { m_data = 0; m_size = 0; ++g_dtors; }
    float* m_data; unsigned int m_size; unsigned int m_pos;
};
int main()
{
    float pool[16];
    unsigned char storage[sizeof(CDelayLine)];
    CDelayLine* d = new (static_cast<void*>(storage)) CDelayLine;
    d->m_data = pool; d->m_size = 16; d->m_pos = 5;
    d->~CDelayLine();
    if (d->m_data != 0 || d->m_size != 0 || d->m_pos != 5 || g_dtors != 1) { std::printf("00c75400_TEST FAIL\n"); return 1; }
    std::printf("00c75400_TEST PASS\n");
    return 0;
}