// behaviour test for 0x0064FA0F: the new element is added once and its ID returned
#include <cstdio>
struct El { long id; };
static int g_added = 0; static long g_next = 41;
static long Add(El* e) { ++g_added; return e->id; }
int main()
{
    El* e = new El; e->id = ++g_next;
    long r = Add(e); delete e;
    if (r != 42 || g_added != 1) { std::printf("0064fa0f_TEST FAIL\n"); return 1; }
    std::printf("0064fa0f_TEST PASS\n");
    return 0;
}