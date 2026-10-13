#include <cstdio>

struct Data { const char* Text; long Length; };
struct Str { Data* PStringData; };
static const void* g_d = 0; static long g_n = -1;
struct Out { virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){} virtual void WriteBytes(const void* d, long n) { g_d = d; g_n = n; }
    void WriteString(Str* s) { Data* d = s->PStringData; if (d) WriteBytes(d->Text, d->Length); else WriteBytes("", 0); } };
int main()
{
    Data d; d.Text = "abc"; d.Length = 3; Str s; s.PStringData = &d; Out o; o.WriteString(&s); if (g_d != d.Text || g_n != 3) { std::printf("009959f0_TEST FAIL\n"); return 1; }
    s.PStringData = 0; o.WriteString(&s); if (g_n != 0 || !g_d || *(const char*)g_d != 0) { std::printf("009959f0_TEST FAIL\n"); return 1; }
    std::printf("009959f0_TEST PASS\n");
    return 0;
}
