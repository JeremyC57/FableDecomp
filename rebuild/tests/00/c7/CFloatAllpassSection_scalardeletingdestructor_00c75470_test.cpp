#include <cstdio>
// CDelayLine scalar deleting destructor: destroy (data/size cleared), free only when bit 0 of the flags is set.
static int g_freed = 0;
struct Line { float* m_data; unsigned int m_size; unsigned int m_pos; };
static Line* DeletingDtor(Line* p, unsigned int flags)
{
    p->m_data = 0; p->m_size = 0;
    if (flags & 1) ++g_freed;
    return p;
}
int main()
{
    float pool[4];
    Line l; l.m_data = pool; l.m_size = 4; l.m_pos = 2;
    if (DeletingDtor(&l, 0) != &l || l.m_data != 0 || l.m_size != 0 || l.m_pos != 2 || g_freed != 0) { std::printf("00c75470_TEST FAIL\n"); return 1; }
    DeletingDtor(&l, 1);
    if (g_freed != 1) { std::printf("00c75470_TEST FAIL\n"); return 1; }
    std::printf("00c75470_TEST PASS\n");
    return 0;
}