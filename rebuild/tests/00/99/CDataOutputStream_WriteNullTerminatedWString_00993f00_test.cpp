#include <cstdio>

struct WS { const wchar_t* t; const wchar_t* GetText() { return t; } long GetLength() { return 3; } };
static const void* g_d = 0; static long g_n = 0;
struct Out { virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){} virtual void WriteBytes(const void* d, long n) { g_d = d; g_n = n; }
    void Write(WS* s) { const wchar_t* c = s->GetText(); long n = (s->GetLength() + 1) * 2; WriteBytes(c, n); } };
int main()
{
    wchar_t buf[4] = { 65, 66, 67, 0 }; WS s; s.t = buf; Out o; o.Write(&s);
    if (g_d != buf || g_n != 8) { std::printf("00993f00_TEST FAIL\n"); return 1; }
    std::printf("00993f00_TEST PASS\n");
    return 0;
}