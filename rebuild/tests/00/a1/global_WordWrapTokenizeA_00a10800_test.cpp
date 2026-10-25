// Behaviour test for 0x00A10800 WordWrap_TokenizeA (count mode and token-array mode, with stand-in code-page conversions
// and a stand-in break rule: a break is allowed in front of a space).
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned short WCh;

static int ToWide(const char* text, int count, WCh* wide, int wideCount)
{
    int n = count < 0 ? (int)std::strlen(text) + 1 : count;
    if (wide == 0) return n;
    if (wideCount < n) return 0;
    for (int i = 0; i < n; ++i) wide[i] = (WCh)(unsigned char)text[i];
    return n;
}
static int ToMulti(const WCh* wide, int count, char* out, int outCount)
{
    if (out == 0) return count;
    if (outCount < count) return 0;
    for (int i = 0; i < count; ++i) out[i] = (char)wide[i];
    return count;
}
static int WLen(const WCh* p) { int n = 0; while (p[n] != 0) ++n; return n; }
static bool CanBreakAt(const WCh* p, const WCh*) { return *p == ' '; }

static long TokenizeA(const char* text, char** tokens)
{
    if (text == 0)
        return 0;
    long count = 0;
    int length = ToWide(text, -1, 0, 0);
    WCh* wide = (WCh*)std::malloc(length * sizeof(WCh));
    if (ToWide(text, -1, wide, length) != 0) {
        const WCh* p = wide + 1;
        const WCh* start = wide;
        int wideLength = WLen(wide);
        if (wideLength > 0) {
            int remaining = wideLength;
            do {
                if (CanBreakAt(p, 0) || *p == 0) {
                    if (tokens != 0) {
                        int tokenLength = (int)(p - start);
                        int size = ToMulti(start, tokenLength, 0, 0);
                        *tokens = (char*)std::malloc(size + 1);
                        ToMulti(start, tokenLength, *tokens, size);
                        (*tokens)[size] = 0;
                        ++tokens;
                    } else
                        ++count;
                    start = p;
                }
                ++p;
            } while (--remaining != 0);
        }
    }
    std::free(wide);
    return count;
}

int main()
{
    int ok = 1;
    if (TokenizeA(0, 0) != 0) ok = 0;
    if (TokenizeA("", 0) != 0) ok = 0;
    if (TokenizeA("one", 0) != 1) ok = 0;
    if (TokenizeA("one two  three", 0) != 4) ok = 0;   // "one", " two", " ", " three"
    char* tok[8] = { 0 };
    if (TokenizeA("one two  three", tok) != 0) ok = 0;  // array mode returns 0
    const char* want[] = { "one", " two", " ", " three" };
    for (int i = 0; i < 4; ++i)
        if (tok[i] == 0 || std::strcmp(tok[i], want[i]) != 0) ok = 0;
    if (tok[4] != 0) ok = 0;
    for (int i = 0; i < 8; ++i) std::free(tok[i]);
    if (!ok) { std::printf("00a10800_TEST FAIL\n"); return 1; }
    std::printf("00a10800_TEST PASS\n");
    return 0;
}