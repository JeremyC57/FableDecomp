#include <cstdio>
#include <string.h>
class CCharString { public:
    const char* text; long length;
    CCharString() : text(""), length(0) { }
    CCharString(const char* t, long l) : text(t), length(l == -1 ? (long)strlen(t) : l) { }
};
static char g_blob[] = { 5, 0, 0, 0, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0, 3, 0, 0, 0, 0x61, 0x62, 0x63, 0 };   // "hello" at 4, "abc" at 14, each behind a 4-byte length
class CMemoryDataOutputStream { public: void* PeekData() { return g_blob; } };
struct CDefStringTable {
    unsigned char StringMap[0xc]; CMemoryDataOutputStream StringStream;
    CCharString GetString(long index) { if (index == -1) return CCharString(); const char* text = (const char*)StringStream.PeekData() + index + 4; return CCharString(text, -1); }
};
int main()
{
    CDefStringTable t;
    CCharString a = t.GetString(-1), b = t.GetString(0), c = t.GetString(10);
    if (a.length != 0 || strcmp(b.text, "hello") != 0 || b.length != 5 || strcmp(c.text, "abc") != 0 || (char*)&t.StringStream - (char*)&t != 0xc) { std::printf("009d49b0_TEST FAIL\n"); return 1; }
    std::printf("009d49b0_TEST PASS\n"); return 0;
}