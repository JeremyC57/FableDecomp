// Behaviour test for NUISystem::CText::ForceWidth (0x0054FB10): the value is rebuilt from its leading characters
// while the accumulated glyph widths stay within the given width, the first character that overflows ending the
// run; the fitted string goes back through the virtual setter with both flags set.
#include <cstdio>
#include <cstring>
static int g_strings = 0;
class CWideString {
public:
    unsigned short* PStringData; unsigned long m_length;
    CWideString() : m_length(0) { ++g_strings; PStringData = new unsigned short[64]; PStringData[0] = 0; }
    ~CWideString() { --g_strings; delete[] PStringData; }
    unsigned long GetLength() const { return m_length; }
    operator const unsigned short*() const { return PStringData; }
    CWideString& operator+=(unsigned short c) { PStringData[m_length++] = c; PStringData[m_length] = 0; return *this; }
    unsigned short operator[](unsigned long i) const { return ((const unsigned short*)*this)[i]; }
};
static unsigned long GetCharWidthW(unsigned short c) { return c == 'W' ? 20 : c == 'i' ? 4 : 10; }
static int g_sets = 0; static unsigned short g_set[64]; static unsigned long g_setLength = 0; static bool g_setA = false, g_setB = false;
namespace NUISystem {
class CText {
public:
    virtual void SetValue(const CWideString& value, bool a, bool b) { ++g_sets; g_setLength = value.GetLength(); std::memcpy(g_set, (const unsigned short*)value, (g_setLength + 1) * 2); g_setA = a; g_setB = b; }
    unsigned char _pad04[0x15c - 0x04];
    CWideString   Value;
    void ForceWidth(float width);
};
}
void NUISystem::CText::ForceWidth(float width)
{
    CWideString fitted;
    float accumulated = 0.0f;
    for (unsigned long i = 0; i != Value.GetLength(); ++i) {
        accumulated += (float)GetCharWidthW(Value[i]);
        if (accumulated > width)
            break;
        fitted += Value[i];
    }
    SetValue(fitted, true, true);
}
static int fail(const char* why) { std::printf("0054fb10_TEST FAIL: %s\n", why); return 1; }
static bool setIs(const char* s) { unsigned long n = std::strlen(s); if (g_setLength != n) return false; for (unsigned long i = 0; i < n; ++i) if (g_set[i] != (unsigned short)s[i]) return false; return g_set[n] == 0; }
int main()
{
    static NUISystem::CText text;
    const char* src = "Wiiab";                           // widths 20 4 4 10 10
    for (const char* p = src; *p; ++p) text.Value += (unsigned short)*p;
    { // 1. 38 fits W,i,i (28) and b... 28 + 10 = 38 fits 'a', 48 > 38 stops before 'b'
        text.ForceWidth(38.0f);
        if (g_sets != 1 || !setIs("Wiia") || !g_setA || !g_setB) return fail("cut at the overflowing character");
    }
    { // 2. everything fits exactly (48)
        text.ForceWidth(48.0f);
        if (!setIs("Wiiab")) return fail("exact fit keeps all");
    }
    { // 3. nothing fits: the first glyph overflows
        text.ForceWidth(19.0f);
        if (g_setLength != 0 || g_set[0] != 0) return fail("empty result");
    }
    { // 4. empty value
        static NUISystem::CText empty; empty.ForceWidth(100.0f);
        if (g_setLength != 0) return fail("empty value");
    }
    if (g_strings != 2) return fail("local string leaked");
    std::printf("0054fb10_TEST PASS\n");
    return 0;
}
