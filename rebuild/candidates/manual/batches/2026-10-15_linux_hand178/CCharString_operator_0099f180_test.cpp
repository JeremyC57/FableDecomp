#include <cstdio>
#include <cstring>
struct Data { char buf[32]; int NoRefs; };
static Data g_pool;
static int g_unshare = 0;
struct CStr {
    Data* p;
    CStr& operator+=(char c)
    {
        if (p) { ++g_unshare; int n = (int)std::strlen(p->buf); p->buf[n] = c; p->buf[n + 1] = 0; return *this; }
        char text[2]; text[0] = c; text[1] = 0;
        if (c != 0) { std::strcpy(g_pool.buf, text); g_pool.NoRefs = 1; p = &g_pool; }
        return *this;
    }
};
int main()
{
    CStr s; s.p = 0;
    s += '\0'; if (s.p) { std::printf("0099f180_TEST FAIL\n"); return 1; }
    s += 'a'; if (!s.p || std::strcmp(s.p->buf, "a") || g_pool.NoRefs != 1) { std::printf("0099f180_TEST FAIL\n"); return 1; }
    s += 'b'; if (std::strcmp(s.p->buf, "ab") || g_unshare != 1) { std::printf("0099f180_TEST FAIL\n"); return 1; }
    std::printf("0099f180_TEST PASS\n");
    return 0;
}
