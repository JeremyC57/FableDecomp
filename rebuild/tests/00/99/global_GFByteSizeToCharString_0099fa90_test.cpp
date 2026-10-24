// Behaviour test for 0x0099FA90 GFByteSizeToCharString: below 4096 -> "%d bytes", below 1 MB -> KB, else MB,
// the KB/MB value printed with `decimals` places (GFFloatToCharString modelled as "%.*f").
#include <cstdio>
#include <cstring>

struct CText {
    char s[64];
    CText() { s[0] = 0; }
};
static CText operator+(const CText& left, const char* right)
{
    CText r;
    std::strcpy(r.s, left.s);
    std::strcat(r.s, right);
    return r;
}
static CText GFIntToCharString(long value) { CText r; std::sprintf(r.s, "%d", (int)value); return r; }
static CText GFFloatToCharString(double value, long decimals) { CText r; std::sprintf(r.s, "%.*f", (int)decimals, value); return r; }

static CText GFByteSizeToCharString(unsigned int size, long decimals)
{
    if (size >= 4096) {
        if (size >= 1048576)
            return GFFloatToCharString(size / 1048576.0f, decimals) + " MB";
        return GFFloatToCharString(size / 1024.0f, decimals) + " KB";
    }
    return GFIntToCharString(size) + " bytes";
}

static int Check(unsigned int size, long decimals, const char* want)
{
    CText t = GFByteSizeToCharString(size, decimals);
    return std::strcmp(t.s, want) == 0;
}

int main()
{
    if (!Check(0, 2, "0 bytes") || !Check(4095, 2, "4095 bytes") || !Check(4096, 2, "4.00 KB") ||
        !Check(524288, 1, "512.0 KB") || !Check(1048576, 0, "1 MB") || !Check(3145728 + 524288, 1, "3.5 MB") ||
        !Check(0xFFFFFFFFu, 0, "4096 MB")) {
        std::printf("0099fa90_TEST FAIL\n");
        return 1;
    }
    std::printf("0099fa90_TEST PASS\n");
    return 0;
}