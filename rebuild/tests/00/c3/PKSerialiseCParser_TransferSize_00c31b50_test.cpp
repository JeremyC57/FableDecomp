// Behaviour test for 0x00C31B50 PKSerialiseCParser::TransferSize (model): forwards the pointer with a 4-byte count.
#include <cstdio>
static void* g_p = 0; static unsigned long g_n = 0;
struct Parser {
    void* r; void* w;
    bool Transfer(void* d, unsigned long n) { g_p = d; g_n = n; return n == 4; }
    bool TransferSize(unsigned long* size) { return Transfer(size, 4); }
};
int main()
{
    Parser p; p.r = 0; p.w = 0;
    unsigned long v = 7;
    int ok = p.TransferSize(&v) && g_p == &v && g_n == 4;
    std::printf(ok ? "00c31b50_TEST PASS\n" : "00c31b50_TEST FAIL\n");
    return ok ? 0 : 1;
}
