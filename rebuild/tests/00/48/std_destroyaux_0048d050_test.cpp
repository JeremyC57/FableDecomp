// behaviour test for 0x0048D050: the deque iterator destroy walk visits every element once
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct It { long* cur; long* first; long* last; long** node; };
static int Walk(It first, It last) { int n = 0; while (first.cur != last.cur) { ++n; if (++first.cur == first.last) { ++first.node; first.first = *first.node; first.last = first.first + 4; first.cur = first.first; } } return n; }
int main()
{
    long a[4], b[4]; long* nodes[2] = { a, b };
    It f; f.node = nodes; f.first = a; f.last = a + 4; f.cur = a + 1;
    It l; l.node = nodes + 1; l.first = b; l.last = b + 4; l.cur = b + 2;
    if (Walk(f, l) != 5 || Walk(f, f) != 0) { std::printf("0048d050_TEST FAIL\n"); return 1; }
    std::printf("0048d050_TEST PASS\n");
    return 0;
}