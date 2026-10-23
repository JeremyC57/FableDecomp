// Behaviour test for CTCHeroOnlineScoreboard::GetCachedMemberLogosAsString (0055ea80): count line, then per entry its id and the cached logo extension (or "0")
#include <cstdio>
#include <cstring>

struct Str {
    char buf[512];
    int len;
    Str() : len(0) { buf[0] = 0; }
    Str(const char* s) : len(0) { buf[0] = 0; add(s); }
    Str& add(const char* s) { while (*s && len < 511) buf[len++] = *s++; buf[len] = 0; return *this; }
    Str& add(const Str& s) { return add(s.buf); }
    Str& addInt(int v) { char t[32]; std::sprintf(t, "%d", v); return add(t); }
    Str sub(int start, int count) const
    {
        Str r;
        if (start < 0) start = 0;
        for (int i = start; i < len && i < start + count; ++i) r.buf[r.len++] = buf[i];
        r.buf[r.len] = 0;
        return r;
    }
    bool eq(const char* s) const { return std::strcmp(buf, s) == 0; }
};

struct Entry { int id; const char* cachedFile; };   // cachedFile: what FindFirstFile returns for SaveDir\MemberData\<id>\logo.*, or 0

static Str BuildLogoString(const Entry* entries, int count)
{
    Str result;
    result.addInt(count).add("\r\n");
    for (int i = 0; i < count; ++i) {
        result.addInt(entries[i].id).add("\r\n");
        Str path("SAVE\\");
        path.add("MemberData\\").addInt(entries[i].id).add("\\logo.*");
        Str extension("0");
        if (entries[i].cachedFile) {
            Str name(entries[i].cachedFile);
            int start = path.len - 1;
            Str file = name.sub(start, name.len);
            extension = file.sub(0, file.len - 4);
        }
        result.add(extension).add("\r\n");
    }
    return result;
}

int main()
{
    Entry entries[3] = { { 7, "SAVE\\MemberData\\7\\logo.png.tmp" }, { 12, 0 }, { 3, "SAVE\\MemberData\\3\\logo.jpg.tmp" } };
    Str s = BuildLogoString(entries, 3);
    if (!s.eq("3\r\n7\r\npng\r\n12\r\n0\r\n3\r\njpg\r\n")) { std::printf("0055ea80_TEST FAIL\n"); return 1; }
    Str empty = BuildLogoString(entries, 0);
    if (!empty.eq("0\r\n")) { std::printf("0055ea80_TEST FAIL\n"); return 1; }
    std::printf("0055ea80_TEST PASS\n");
    return 0;
}