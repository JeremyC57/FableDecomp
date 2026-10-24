// Behaviour test for 0x00A0DAC0 FormatNumberWithSeparators: decimal text with a separator every three digits, shifted in place from the end.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static unsigned short* Stub_itow(int value, unsigned short* text, int radix)
{
    char tmp[16];
    std::sprintf(tmp, "%d", value);
    int i = 0;
    for (; tmp[i]; ++i) text[i] = (unsigned short)tmp[i];
    text[i] = 0;
    (void)radix;
    return text;
}
static unsigned int Stub_wcslen(const unsigned short* text) { unsigned int n = 0; while (text[n]) ++n; return n; }
static unsigned short GetThousandsSeparator() { return ','; }

void __fastcall FormatNumberWithSeparators(long value, unsigned short* text)
{
    Stub_itow(value, text, 10);
    long length = Stub_wcslen(text);
    long separators = (length - 1) / 3;
    if (separators > 0) {
        unsigned short separator = GetThousandsSeparator();
        unsigned short* dest = text + length + separators;
        unsigned short* src = text + length;
        for (long i = 0; i < length; ++i, --dest, --src) {
            *dest = *src;
            if (i != 0 && i % 3 == 0)
                *--dest = separator;
        }
    }
}

static bool Same(const unsigned short* a, const char* b)
{
    while (*b) { if (*a != (unsigned char)*b) return false; ++a; ++b; }
    return *a == 0;
}

int main()
{
    unsigned short buf[32];
    FormatNumberWithSeparators(0, buf);        if (!Same(buf, "0"))             { std::printf("00a0dac0_TEST FAIL\n"); return 1; }
    FormatNumberWithSeparators(999, buf);      if (!Same(buf, "999"))           { std::printf("00a0dac0_TEST FAIL\n"); return 1; }
    FormatNumberWithSeparators(1000, buf);     if (!Same(buf, "1,000"))         { std::printf("00a0dac0_TEST FAIL\n"); return 1; }
    FormatNumberWithSeparators(12345, buf);    if (!Same(buf, "12,345"))        { std::printf("00a0dac0_TEST FAIL\n"); return 1; }
    FormatNumberWithSeparators(1234567, buf);  if (!Same(buf, "1,234,567"))     { std::printf("00a0dac0_TEST FAIL\n"); return 1; }
    FormatNumberWithSeparators(-1234, buf);    if (!Same(buf, "-1,234"))        { std::printf("00a0dac0_TEST FAIL\n"); return 1; }
    std::printf("00a0dac0_TEST PASS\n");
    return 0;
}