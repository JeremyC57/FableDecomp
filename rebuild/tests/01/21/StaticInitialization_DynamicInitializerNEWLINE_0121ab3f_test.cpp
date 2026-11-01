// Behaviour test for 0x0121AB3F `anonymous namespace'::`dynamic initializer for 'NEWLINE'`: before main the TU-local wide
// string is constructed from L"\n" (one character, 0x000A), ready for the weapon-description `+=` users.
#include <cstdio>
typedef unsigned short WCHAR16;
class CWideString { public:
    explicit CWideString(const WCHAR16* text) : Length(0)
    {
        while (text[Length] != 0 && Length < 15) { Text[Length] = text[Length]; ++Length; }
        Text[Length] = 0;
    }
    ~CWideString() { Length = 0; }
    WCHAR16 Text[16];
    long Length;
};
static const WCHAR16 kNewLine[] = { 0x000A, 0 };
namespace {
const CWideString NEWLINE(kNewLine);
}
int main()
{
    if (NEWLINE.Length != 1 || NEWLINE.Text[0] != 0x000A || NEWLINE.Text[1] != 0) { std::printf("0121ab3f_TEST FAIL\n"); return 1; }
    std::printf("0121ab3f_TEST PASS\n");
    return 0;
}