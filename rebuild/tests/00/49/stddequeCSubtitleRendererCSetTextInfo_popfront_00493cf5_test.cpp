// Behaviour test for 0x00493CF5 std::deque<CSubtitleRenderer::CSetTextInfo>::pop_front (model): destroy the front
// record and step the cursor; on the last record of the front node take the slow path (_M_pop_front_aux).
#include <cstdio>
struct CSetTextInfo { int Tag; char Pad[24]; };          // 28 bytes, as the retail stride 0x1C
static int g_destroyed = 0, g_aux = 0;
static int g_lastTag = -1;
struct Iter { CSetTextInfo* cur; CSetTextInfo* first; CSetTextInfo* last; CSetTextInfo** node; };
struct Deque {
    Iter start, finish;
    void destroy(CSetTextInfo* p) { ++g_destroyed; g_lastTag = p->Tag; }
    void pop_front_aux() { destroy(start.cur); ++g_aux; ++start.node; start.first = *start.node; start.last = start.first + 4; start.cur = start.first; }
    void pop_front()
    {
        if (start.cur != start.last - 1) { destroy(start.cur); ++start.cur; }
        else pop_front_aux();
    }
};
int main()
{
    CSetTextInfo a[4], b[4];
    for (int i = 0; i < 4; ++i) { a[i].Tag = i; b[i].Tag = 10 + i; }
    CSetTextInfo* map[2] = { a, b };
    Deque d;
    d.start.cur = a; d.start.first = a; d.start.last = a + 4; d.start.node = map;
    d.finish = d.start;
    int ok = 1;
    for (int i = 0; i < 3; ++i) { d.pop_front(); ok &= g_lastTag == i && d.start.cur == a + i + 1 && g_aux == 0; }
    d.pop_front();                                        // last record of the front node: slow path
    ok &= g_lastTag == 3 && g_aux == 1 && d.start.cur == b && d.start.node == map + 1 && d.start.last == b + 4;
    d.pop_front();
    ok &= g_lastTag == 10 && d.start.cur == b + 1 && g_destroyed == 5;
    std::printf(ok ? "00493cf5_TEST PASS\n" : "00493cf5_TEST FAIL\n");
    return ok ? 0 : 1;
}
