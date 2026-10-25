// Behaviour test for LHNetworkLib::LHHttpHeaders::GetHeaderString() (0x0083FCE0): HeaderString is cleared, the default
// "User-Agent: ..." line is appended to the header list when no line starts (case-insensitively) with "User-Agent",
// then every header line is appended to HeaderString followed by CRLF and the result is returned (a copy of
// HeaderString). Calling it twice yields the same block (the default agent is added only once).
#include <cstdio>
#include <cstring>
#include <cctype>

static const char* kDefaultUserAgent = "User-Agent: Lionhead Studios HTTP Library";

static bool StartsWithNoCase(const char* text, const char* prefix)
{
    for (; *prefix; ++text, ++prefix)
        if (!*text || tolower((unsigned char)*text) != tolower((unsigned char)*prefix))
            return false;
    return true;
}

struct Headers {
    const char* lines[8];
    int count;
    char headerString[512];
};

static bool HasHeader(const Headers& h, const char* name)
{
    for (int i = 0; i < h.count; ++i)
        if (StartsWithNoCase(h.lines[i], name))
            return true;
    return false;
}

static void GetHeaderString(Headers& h, char* out)
{
    h.headerString[0] = 0;
    if (!HasHeader(h, "User-Agent"))
        h.lines[h.count++] = kDefaultUserAgent;
    for (int i = 0; i < h.count; ++i) {
        strcat(h.headerString, h.lines[i]);
        strcat(h.headerString, "\r\n");
    }
    strcpy(out, h.headerString);
}

int main()
{
    int fails = 0;
    char out[512];

    // no user agent: the default one is appended after the existing lines
    {
        Headers h = { { "Host: example", "Accept: */*" }, 2, "stale" };
        GetHeaderString(h, out);
        if (h.count != 3 || strcmp(out, "Host: example\r\nAccept: */*\r\nUser-Agent: Lionhead Studios HTTP Library\r\n"))
            ++fails;
        if (strcmp(out, h.headerString)) ++fails;
        GetHeaderString(h, out);                // second call: no second default agent
        if (h.count != 3) ++fails;
    }
    // an existing (lower-case) user agent is kept and no default is added
    {
        Headers h = { { "user-agent: Me", "Host: x" }, 2, "" };
        GetHeaderString(h, out);
        if (h.count != 2 || strcmp(out, "user-agent: Me\r\nHost: x\r\n")) ++fails;
    }
    // an empty list gets just the default agent
    {
        Headers h = { { 0 }, 0, "junk" };
        GetHeaderString(h, out);
        if (h.count != 1 || strcmp(out, "User-Agent: Lionhead Studios HTTP Library\r\n")) ++fails;
    }

    if (fails) { printf("0083fce0_TEST FAIL\n"); return 1; }
    printf("0083fce0_TEST PASS\n");
    return 0;
}