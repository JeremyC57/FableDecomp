#include <cstdio>
static char g_written[8]; static int g_n = 0;
struct CDataOutputStream { void WriteCBYTE(char v) { g_written[g_n++] = v; } };
struct CDefClassBase {
    virtual void v00() { }
    unsigned char _body[0x20];
    unsigned char Setup : 1; unsigned char Template : 1; unsigned char DefaultValsApplied : 1;
    virtual void InternalSaveBinary(CDataOutputStream& s) { s.WriteCBYTE(Setup); s.WriteCBYTE(Template); s.WriteCBYTE(DefaultValsApplied); }
};
int main()
{
    CDefClassBase d; d.Setup = 1; d.Template = 0; d.DefaultValsApplied = 1; CDataOutputStream s;
    d.InternalSaveBinary(s);
    if (g_n != 3 || g_written[0] != 1 || g_written[1] != 0 || g_written[2] != 1) { std::printf("009acab0_TEST FAIL\n"); return 1; }
    std::printf("009acab0_TEST PASS\n"); return 0;
}
