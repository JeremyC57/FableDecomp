#include <cstdio>
#include <cstring>
// Behaviour test for CParsedItem::ConvertToString (0x009B98B0): integer, float, string and null kinds each give their text.
enum EParsedItemType { PIT_NONE, PIT_STRING, PIT_QUOTED_STRING, PIT_INTEGER, PIT_FLOAT };
struct CCharString {
    char m_text[32];
    CCharString(const CCharString& other) { std::strcpy(m_text, other.m_text); }
    CCharString(const char* text, long) { std::strcpy(m_text, text); }
    static CCharString FormatDouble(long digits, double value) { char b[32]; std::sprintf(b, "%.*f", (int)digits, value); return CCharString(b, -1); }
};
static CCharString GFIntToCharString(long value) { char b[32]; std::sprintf(b, "%ld", value); return CCharString(b, -1); }
struct CParsedItem {
    EParsedItemType Type; union { long Integer; float Float; }; CCharString String;
    CParsedItem() : String("abc", -1) {}
    CCharString ConvertToString() const
    {
        switch (Type) {
        case PIT_INTEGER: return GFIntToCharString(Integer);
        case PIT_FLOAT: return CCharString::FormatDouble(6, Float);
        case PIT_STRING: case PIT_QUOTED_STRING: return String;
        default: return CCharString("NULL", -1);
        }
    }
};
int main()
{
    CParsedItem p; int ok = 1;
    p.Type = PIT_INTEGER; p.Integer = -42; if (std::strcmp(p.ConvertToString().m_text, "-42")) ok = 0;
    p.Type = PIT_FLOAT; p.Float = 1.5f; if (std::strcmp(p.ConvertToString().m_text, "1.500000")) ok = 0;
    p.Type = PIT_STRING; if (std::strcmp(p.ConvertToString().m_text, "abc")) ok = 0;
    p.Type = PIT_QUOTED_STRING; if (std::strcmp(p.ConvertToString().m_text, "abc")) ok = 0;
    p.Type = PIT_NONE; if (std::strcmp(p.ConvertToString().m_text, "NULL")) ok = 0;
    std::printf(ok ? "009b98b0_TEST PASS\n" : "009b98b0_TEST FAIL\n"); return ok ? 0 : 1;
}