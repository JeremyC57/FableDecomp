// Behaviour test for 0x007B5270 back_insert_iterator<deque<CCrimeWaitingForBribe> >::operator=: append at the finish
// cursor while the node has room for one more, else the aux slow path.
#include <cstdio>
struct Rec { int a, b; };
static int g_aux = 0;
struct It { Rec* cur; Rec* first; Rec* last; Rec** node; };
struct Deque {
    It start, finish;
    void aux(const Rec& t) { ++g_aux; *finish.cur = t; }
    void push_back(const Rec& t) { if (finish.cur != finish.last - 1) { *finish.cur = t; ++finish.cur; } else aux(t); }
};
struct Inserter { Deque* c; Inserter& operator=(const Rec& v) { c->push_back(v); return *this; } };
int main()
{
    Rec buf[16];
    Deque d; d.finish.first = buf; d.finish.cur = buf + 14; d.finish.last = buf + 16;
    Inserter it; it.c = &d;
    Rec r1 = { 5, 6 }, r2 = { 7, 8 };
    it = r1;
    if (d.finish.cur != buf + 15 || buf[14].a != 5 || g_aux != 0) { std::printf("007b5270_TEST FAIL\n"); return 1; }
    it = r2;   // last slot of the node: aux path
    if (g_aux != 1 || buf[15].b != 8) { std::printf("007b5270_TEST FAIL\n"); return 1; }
    std::printf("007b5270_TEST PASS\n");
    return 0;
}