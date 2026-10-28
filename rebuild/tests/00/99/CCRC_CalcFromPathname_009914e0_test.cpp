// Behaviour test for 0x009914E0 CCRC::CalcFromPathname: the CRC covers only the lower-cased "name.ext" part, so paths
// differing in directory, drive or letter case hash alike; an empty name/ext hashes the empty string with seed 0.
#include <cstdio>
#include <cstring>
#include <cctype>
static const char g_EmptyString[] = "";
class CCharString { public:
    char buf[64]; bool null;
    CCharString() : null(true) { buf[0] = 0; }
    CCharString(const char* s) : null(false) { std::strncpy(buf, s, 63); buf[63] = 0; }
    const CCharString& operator=(const CCharString& o) { std::memcpy(buf, o.buf, 64); null = o.null; return *this; }
    CCharString ToLower() const { CCharString r(*this); for (char* p = r.buf; *p; ++p) *p = (char)std::tolower(*p); return r; }
    const char* GetPString() const { return null ? g_EmptyString : buf; }
    unsigned int GetLength() const { return null ? 0 : (unsigned int)std::strlen(buf); }
};
static CCharString operator+(const CCharString& a, const CCharString& b)
{
    CCharString r(a.GetPString()); std::strncat(r.buf, b.GetPString(), 63 - std::strlen(r.buf)); r.null = false; return r;
}
static void SplitPath(const CCharString& path, CCharString*, CCharString*, CCharString* name, CCharString* ext)
{
    const char* s = path.GetPString(); const char* slash = s;
    for (const char* p = s; *p; ++p) if (*p == '\\' || *p == '/' || *p == ':') slash = p + 1;
    const char* dot = std::strrchr(slash, '.');
    size_t n = dot ? (size_t)(dot - slash) : std::strlen(slash);
    char tmp[64]; std::memcpy(tmp, slash, n); tmp[n] = 0;
    *name = CCharString(tmp); *ext = CCharString(dot ? dot : "");
}
static unsigned long g_lastSeed = 99;
static unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    g_lastSeed = seed;
    unsigned long crc = ~seed; const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) { crc ^= p[i]; for (int k = 0; k < 8; ++k) crc = (crc >> 1) ^ (0xEDB88320UL & (0UL - (crc & 1))); }
    return ~crc & 0xFFFFFFFFUL;
}
static unsigned long CalcFromString(const CCharString& s) { return Calc(0, s.GetPString(), s.GetLength()); }
static unsigned long CalcFromPathname(const CCharString& pathname)
{
    CCharString file; CCharString name; CCharString ext;
    SplitPath(pathname, 0, 0, &name, &ext);
    file = name + ext;
    return CalcFromString(file.ToLower());
}
int main()
{
    int fails = 0;
    unsigned long a = CalcFromPathname(CCharString("C:\\Data\\Levels\\Foo.TXT"));
    unsigned long b = CalcFromPathname(CCharString("d:/other/foo.txt"));
    unsigned long c = Calc(0, "foo.txt", 7);
    if (a != b || a != c || g_lastSeed != 0) ++fails;
    if (CalcFromPathname(CCharString("x/bar.txt")) == a) ++fails;
    if (CalcFromPathname(CCharString("dir\\")) != Calc(0, "", 0)) ++fails;
    std::printf(fails ? "009914e0_TEST FAIL\n" : "009914e0_TEST PASS\n");
    return fails ? 1 : 0;
}