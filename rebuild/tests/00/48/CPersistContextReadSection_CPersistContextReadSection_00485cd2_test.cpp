// behaviour test for 0x00485CD2
#include <stdio.h>
static int g_log[8]; static int g_n = 0; static void* g_addr[4];
class CPersistContext {
public:
    unsigned char Data[0x2c];                            // Ego_r.pdb 44 bytes, no constructor code here
};
class CMemoryDataInputStream {
public:
    unsigned char Data[0x1c];
    CMemoryDataInputStream() { g_log[g_n++] = 1; g_addr[1] = this; }
};
class CStringParser {
public:
    unsigned char Data[0x28];
    CStringParser() { g_log[g_n++] = 2; g_addr[2] = this; }
};
class CCharString {
public:
    void* PData;
    CCharString() { g_log[g_n++] = 3; g_addr[3] = this; }
};
class CPersistContextReadSection {
public:
    CPersistContext        Context;                      // +0x00
    CMemoryDataInputStream InputStream;                  // +0x2c
    CStringParser          Parser;                       // +0x48
    CCharString            Text;                         // +0x70
    CPersistContextReadSection();
};
CPersistContextReadSection::CPersistContextReadSection()
{
}

int main()
{
    CPersistContextReadSection r; char* b = (char*)&r;
    int ok = g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3
          && (char*)g_addr[1] - b == 0x2c && (char*)g_addr[2] - b == 0x48 && (char*)g_addr[3] - b == 0x70;
    printf(ok ? "00485cd2_TEST PASS\n" : "00485cd2_TEST FAIL\n");
    return ok ? 0 : 1;
}