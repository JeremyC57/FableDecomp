#include <cstdio>
// Behaviour model: the old child is deleted unless it is the shared blocked node; the new one is stored.
static int deleted = 0;
struct N { virtual ~N() { ++deleted; } };
static N blocked;
struct Q { N* c; void Set(N* n) { if (c != &blocked) delete c; c = n; } };
int main()
{
    Q q; q.c = &blocked; N* a = new N;
    q.Set(a);
    if (deleted != 0 || q.c != a) { std::printf("00a75d60_TEST FAIL\n"); return 1; }
    q.Set(&blocked);
    if (deleted != 1 || q.c != &blocked) { std::printf("00a75d60_TEST FAIL\n"); return 1; }
    q.c = 0; q.Set(&blocked);
    if (deleted != 1) { std::printf("00a75d60_TEST FAIL\n"); return 1; }
    std::printf("00a75d60_TEST PASS\n");
    return 0;
}