#include <cstdio>

// test stand-in for CWideString: a fixed-capacity UTF-16 buffer with the engine operations the rows call
class CWideString {
public:
    enum { CAP = 128 };
    CWideString() { Len = 0; Data[0] = 0; }
    CWideString(const unsigned short* text) { Assign(text); }
    CWideString(const CWideString& other) { Assign(other.Data); }
    ~CWideString() { ++Destroyed; }
    CWideString& operator=(const CWideString& other) { Assign(other.Data); return *this; }
    bool Contains(const CWideString& text) const { return Find(text) >= 0; }
    void Replace(const CWideString& search, const CWideString& replacement)
    {
        int at;
        while ((at = Find(search)) >= 0) {
            unsigned short out[CAP];
            int n = 0, i;
            for (i = 0; i < at; ++i) out[n++] = Data[i];
            for (i = 0; i < replacement.Len; ++i) out[n++] = replacement.Data[i];
            for (i = at + search.Len; i < Len; ++i) out[n++] = Data[i];
            out[n] = 0;
            Assign(out);
        }
    }
    void Format(const unsigned short* fmt, int value)     // "%d"-only stand-in
    {
        (void)fmt;
        char digits[16];
        int n = std::sprintf(digits, "%d", value);
        for (int i = 0; i <= n; ++i) Data[i] = (unsigned short)(unsigned char)digits[i];
        Len = n;
    }
    bool Equals(const char* ascii) const
    {
        int i = 0;
        for (; ascii[i]; ++i) if (i >= Len || Data[i] != (unsigned short)(unsigned char)ascii[i]) return false;
        return i == Len;
    }
    int Find(const CWideString& s) const
    {
        if (s.Len == 0) return -1;
        for (int i = 0; i + s.Len <= Len; ++i) {
            int j = 0;
            while (j < s.Len && Data[i + j] == s.Data[j]) ++j;
            if (j == s.Len) return i;
        }
        return -1;
    }
    void Assign(const unsigned short* text)
    {
        Len = 0;
        while (text[Len] && Len < CAP - 1) { Data[Len] = text[Len]; ++Len; }
        Data[Len] = 0;
    }
    static int Destroyed;
    unsigned short Data[CAP];
    int Len;
};
int CWideString::Destroyed = 0;

static CWideString W(const char* ascii)
{
    unsigned short buf[CWideString::CAP];
    int i = 0;
    for (; ascii[i] && i < CWideString::CAP - 1; ++i) buf[i] = (unsigned short)(unsigned char)ascii[i];
    buf[i] = 0;
    return CWideString(buf);
}

namespace NGameText {
const unsigned short g_FirstPlaceholder[] = { '%', '1', 0 };

bool ConstructString(CWideString& output, const CWideString& replacement)
{
    CWideString key(g_FirstPlaceholder);
    if (output.Contains(key)) {
        output.Replace(key, replacement);
        return true;
    }
    return false;
}
}

int main()
{
    CWideString out = W("You found %1 gold.");
    if (!NGameText::ConstructString(out, W("25")) || !out.Equals("You found 25 gold.")) { std::printf("009c7980_TEST FAIL\n"); return 1; }
    CWideString none = W("No placeholder here");
    if (NGameText::ConstructString(none, W("x")) || !none.Equals("No placeholder here")) { std::printf("009c7980_TEST FAIL\n"); return 1; }
    CWideString two = W("%1 and %1");
    if (!NGameText::ConstructString(two, W("A")) || !two.Equals("A and A")) { std::printf("009c7980_TEST FAIL\n"); return 1; }
    CWideString other = W("only %2");
    if (NGameText::ConstructString(other, W("B")) || !other.Equals("only %2")) { std::printf("009c7980_TEST FAIL\n"); return 1; }
    std::printf("009c7980_TEST PASS\n");
    return 0;
}