// Behaviour test for LHNetworkLib::LHHttp2::GetHeader(LHHttpHeaderStatus&) (0x00840B60): with no received header lines
// nothing changes; otherwise every received line is appended to the status's header list, the server code is the
// number after the first space of the "ServerCode" header, "Location:" is copied when present, the document size is
// Content-Length (or -1 when absent), and the document encoding becomes chunked only when Transfer-Encoding starts
// (case-insensitively) with "chunked". The header lookup is LHHttpHeaders::GetHeader: the first line that starts
// case-insensitively with the name, its text after the first ':' with leading spaces skipped. Always returns 1000.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

static bool StartsWithNoCase(const char* text, const char* prefix)
{
    for (; *prefix; ++text, ++prefix)
        if (!*text || tolower((unsigned char)*text) != tolower((unsigned char)*prefix))
            return false;
    return true;
}

struct Headers { const char* lines[8]; int count; };

// LHHttpHeaders::GetHeader: value after ':' (spaces skipped), whole line if no ':' past column 0, "" if no match
static void GetHeaderValue(const Headers& h, const char* name, char* out)
{
    out[0] = 0;
    for (int i = 0; i < h.count; ++i) {
        const char* line = h.lines[i];
        if (StartsWithNoCase(line, name)) {
            const char* colon = strchr(line, ':');
            if (colon && colon != line) {
                const char* v = colon + 1;
                while (*v == ' ') ++v;
                strcpy(out, v);
            }
            else
                strcpy(out, line);
            return;
        }
    }
}

struct Status { int serverCode; Headers headers; char location[64]; long documentSize; };
struct Http { Headers received; int documentEncoding; };

static int GetHeader(Http& http, Status& status)
{
    if (http.received.count != 0) {
        for (int i = 0; i < http.received.count; ++i)
            status.headers.lines[status.headers.count++] = http.received.lines[i];
        char value[64];
        GetHeaderValue(status.headers, "ServerCode", value);
        if (value[0]) {
            const char* space = strchr(value, ' ');
            int code = atoi(space ? space + 1 : value);
            status.serverCode = code;
        }
        GetHeaderValue(status.headers, "Location:", value);
        if (value[0]) strcpy(status.location, value);
        http.documentEncoding = 0;
        GetHeaderValue(status.headers, "Content-Length", value);
        status.documentSize = value[0] ? atol(value) : -1;
        GetHeaderValue(status.headers, "Transfer-Encoding", value);
        if (value[0] && StartsWithNoCase(value, "chunked"))
            http.documentEncoding = 1;
    }
    return 1000;
}

int main()
{
    int fails = 0;

    // nothing received: status untouched, still 1000
    {
        Http http = { { { 0 }, 0 }, 7 };
        Status st = { -1, { { 0 }, 0 }, "old", 5 };
        if (GetHeader(http, st) != 1000) ++fails;
        if (st.serverCode != -1 || st.headers.count != 0 || strcmp(st.location, "old") || st.documentSize != 5
            || http.documentEncoding != 7) ++fails;
    }
    // a 302 redirect with a length and chunked encoding
    {
        Http http = { { { "ServerCode: HTTP/1.1 302 Found", "Location: http://host/next", "Content-Length: 1234",
                          "transfer-encoding: Chunked" }, 4 }, 7 };
        Status st = { -1, { { 0 }, 0 }, "", 0 };
        if (GetHeader(http, st) != 1000) ++fails;
        if (st.headers.count != 4 || st.serverCode != 302) ++fails;
        if (strcmp(st.location, "http://host/next") || st.documentSize != 1234 || http.documentEncoding != 1) ++fails;
    }
    // a 200 without length, location or chunking: size -1, encoding reset to 0, location kept
    {
        Http http = { { { "ServerCode: HTTP/1.0 200 OK", "Transfer-Encoding: identity" }, 2 }, 1 };
        Status st = { -1, { { 0 }, 0 }, "keep", 9 };
        GetHeader(http, st);
        if (st.serverCode != 200 || st.documentSize != -1 || http.documentEncoding != 0) ++fails;
        if (strcmp(st.location, "keep") || st.headers.count != 2) ++fails;
    }

    if (fails) { printf("00840b60_TEST FAIL\n"); return 1; }
    printf("00840b60_TEST PASS\n");
    return 0;
}