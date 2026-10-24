// Behaviour test for 0x009E9E30 CDateAndTime::ToString.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int gFrees = 0;
struct CCharString {
    char Text[64];
    CCharString(const char* text) { std::strncpy(Text, text, 63); Text[63] = 0; }
};
struct CTextBuffer {
    char* Data;
    CTextBuffer(unsigned int size) : Data((char*)std::malloc(size)) { std::memset(Data, 0, size); }
    ~CTextBuffer() { if (Data) { ++gFrees; std::free(Data); } }
};
struct CDateAndTime {
    long Year; unsigned int Month, Day, Hour, Minute, Second, Millisecond;
    CCharString ToString() const
    {
        CTextBuffer text(1024);
        std::sprintf(text.Data, "[%02d/%02d/%02d %02d:%02d:%02d]", (int)Day, (int)Month, (int)Year, (int)Hour, (int)Minute, (int)Second);
        CCharString result(text.Data);
        return result;
    }
};

int main()
{
    CDateAndTime t = { 4, 9, 14, 7, 5, 3, 0 };
    CCharString s = t.ToString();
    bool ok = std::strcmp(s.Text, "[14/09/04 07:05:03]") == 0 && gFrees == 1;
    CDateAndTime u = { 2005, 12, 31, 23, 59, 58, 999 };
    ok = ok && std::strcmp(u.ToString().Text, "[31/12/2005 23:59:58]") == 0 && gFrees == 2;
    if (!ok) { std::printf("009e9e30_TEST FAIL\n"); return 1; }
    std::printf("009e9e30_TEST PASS\n");
    return 0;
}