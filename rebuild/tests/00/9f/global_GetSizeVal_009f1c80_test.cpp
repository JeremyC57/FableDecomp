// Behaviour test for 0x009F1C80 GetSizeVal: digits, optional spaces, then an optional k/K (x1024) or m/M (x1048576)
// suffix; parsing stops at end.
#include <cstdio>
#include <cstring>
#include <cctype>

static unsigned int GetSizeVal(const char* p, const char* end)
{
    unsigned int value = 0;
    while (p < end && std::isdigit(*p)) {
        value = value * 10 + (*p - '0');
        ++p;
    }
    while (p < end && std::isspace(*p))
        ++p;
    if (p < end) {
        if (std::tolower(*p) == 'k')
            value <<= 10;
        else if (std::tolower(*p) == 'm')
            value <<= 20;
    }
    return value;
}

static unsigned int Parse(const char* s) { return GetSizeVal(s, s + std::strlen(s)); }

int main()
{
    bool ok = Parse("123") == 123 && Parse("64k") == 64u * 1024 && Parse("16 M") == 16u * 1048576
           && Parse("8  K") == 8u * 1024 && Parse("12x") == 12 && Parse("") == 0 && Parse("k") == 0
           && Parse("  5") == 0 && Parse("7m trailing") == 7u * 1048576;
    const char* s = "32k";
    ok = ok && GetSizeVal(s, s + 2) == 32;                  // the suffix lies past end
    if (!ok) { std::printf("009f1c80_TEST FAIL\n"); return 1; }
    std::printf("009f1c80_TEST PASS\n");
    return 0;
}