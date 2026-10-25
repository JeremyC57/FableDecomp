// Behaviour test for 0099f3b0: CCharString::Append(char, count) -- the block is made unique (or created empty) first, then count copies of c appended
#include <cstdio>
#include <cstring>
static char g_seq[16] = "";
struct Block { char text[32]; unsigned long len; long refs; void Append(char c, unsigned long n) { std::strcat(g_seq, "A"); for (unsigned long i = 0; i < n; ++i) text[len++] = c; text[len] = 0; } };
static Block g_pool[4]; static int g_used = 0;
struct Str {
    Block* p;
    void MakeOwn() { std::strcat(g_seq, "U"); if (!p) { p = &g_pool[g_used++]; p->len = 0; p->text[0] = 0; p->refs = 1; } else if (p->refs > 1) { Block* n = &g_pool[g_used++]; *n = *p; n->refs = 1; --p->refs; p = n; } }
    void Append(char c, unsigned long n) { MakeOwn(); p->Append(c, n); }
};
int main()
{
    Str a; a.p = 0; a.Append('x', 3);
    int ok = std::strcmp(a.p->text, "xxx") == 0 && std::strcmp(g_seq, "UA") == 0;
    Str b; b.p = a.p; a.p->refs = 2; b.Append('y', 2);
    ok = ok && std::strcmp(a.p->text, "xxx") == 0 && std::strcmp(b.p->text, "xxxyy") == 0 && b.p != a.p && a.p->refs == 1;
    a.Append('z', 0); ok = ok && a.p->len == 3;
    std::printf(ok ? "0099f3b0_TEST PASS\n" : "0099f3b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
