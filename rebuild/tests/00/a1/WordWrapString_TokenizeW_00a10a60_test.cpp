// Behaviour test for WordWrap_String::TokenizeW 0x00A10A60. Retail walks 16-bit units starting ONE BYTE past the text
// (`lea edi,[eax+1]`), so the model addresses the text as bytes and reads little-endian 16-bit units at byte offsets
// (portable to a 32-bit wchar_t host). Character classes as in the source (g_WordWrapOptions selects their tables).
#include <cstdio>
#include <cstring>

typedef unsigned short wchar16;
static unsigned long g_WordWrapOptions;

static bool IsNonBeginningChar(wchar16 ch)
{
    if (!(g_WordWrapOptions & 1))
        return false;
    return ch == 0x002C || ch == 0x002E || ch == 0x0021 || ch == 0x003F || ch == 0x3001 || ch == 0x3002;   // subset
}
static bool IsNonEndingChar(wchar16 c)
{
    if (!(g_WordWrapOptions & 1))
        return false;
    return c == 0x0028 || c == 0x005B || c == 0x3008;   // subset
}
static bool IsEastAsianChar(wchar16 c)
{
    if (g_WordWrapOptions & 4) {
        if ((c >= 0x1100 && c <= 0x11FF) || (c >= 0x3130 && c <= 0x318F) || (c >= 0xAC00 && c <= 0xD7A3))
            return false;
    }
    return (c >= 0x1100 && c <= 0x11FF) || (c >= 0x3000 && c <= 0xD7AF) || (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFDC);
}

static unsigned char g_mem[256];                      // byte image: [16] is the text
static wchar16 At(long byteOffset) { return (wchar16)(g_mem[byteOffset] | (g_mem[byteOffset + 1] << 8)); }
static int Wcslen(long t) { int n = 0; while (At(t + 2 * n)) ++n; return n; }

#define WORDWRAP_IS_WHITE_SPACE(c) ((c) == 0x0009 || (c) == 0x000D || (c) == 0x0020 || (c) == 0x3000)
static bool CanBreakLineAt(long text, long start)     // byte offsets; 0 = the null start
{
    if (text == start)
        return false;
    return (WORDWRAP_IS_WHITE_SPACE(At(text)) || IsEastAsianChar(At(text)) || IsEastAsianChar(At(text - 2)))
        && !IsNonBeginningChar(At(text)) && !IsNonEndingChar(At(text - 2));
}

// tokens == 0: count mode; else record each token's (start byte, length) pair.
static int TokenizeW(long text, int* tokenStarts, int* tokenLengths, int* tokenCount)
{
    long cursor = text + 1;
    long tokenStart = text;
    int count = 0;
    for (int remaining = Wcslen(text); remaining > 0; --remaining, cursor += 2) {
        if (CanBreakLineAt(cursor, 0) || At(cursor) == 0) {
            if (tokenStarts) {
                int length = (int)(cursor - tokenStart) >> 1;   // sar 1 of the byte difference
                tokenStarts[*tokenCount] = (int)tokenStart;
                tokenLengths[*tokenCount] = length;
                ++*tokenCount;
            } else
                ++count;
            tokenStart = cursor;
        }
    }
    return count;
}

static void SetText(const wchar16* s)
{
    std::memset(g_mem, 0, sizeof(g_mem));
    for (int i = 0; s[i]; ++i) { g_mem[16 + 2 * i] = (unsigned char)s[i]; g_mem[17 + 2 * i] = (unsigned char)(s[i] >> 8); }
}
static int fail(int line) { std::printf("00a10a60_TEST FAIL (line %d)\n", line); return 1; }

int main()
{
    static const wchar16 ab[] = { 'a', 'b', 0 };
    static const wchar16 empty[] = { 0 };
    static const wchar16 cjk[] = { 0x4E00, 0x4E01, 0x4E02, 0 };

    g_WordWrapOptions = 0;
    SetText(empty);
    if (TokenizeW(16, 0, 0, 0) != 0) return fail(__LINE__);            // skipped loop returns the zero count

    // "ab": the misaligned units are 0x6200 (East Asian range) and 0x0000 -> two breaks
    SetText(ab);
    if (TokenizeW(16, 0, 0, 0) != 2) return fail(__LINE__);

    int starts[8], lengths[8], n = 0;
    if (TokenizeW(16, starts, lengths, &n) != 0 || n != 2) return fail(__LINE__);   // token mode returns 0
    if (starts[0] != 16 || lengths[0] != 0 || starts[1] != 17 || lengths[1] != 1) return fail(__LINE__);

    // ideographs 0x4E00..0x4E02: the misaligned units read 0x014E, 0x024E, 0x004E -- none breaks, none is zero
    SetText(cjk);
    if (TokenizeW(16, 0, 0, 0) != 0) return fail(__LINE__);

    // the Hangul exclusion option does not touch 0x6200, so "ab" still yields two breaks
    g_WordWrapOptions = 4;
    SetText(ab);
    if (TokenizeW(16, 0, 0, 0) != 2) return fail(__LINE__);

    // option 1 enables the non-beginning table: a text whose first misaligned unit is 0x3001 is built directly
    g_WordWrapOptions = 1;
    std::memset(g_mem, 0, sizeof(g_mem));
    g_mem[16] = 0x41; g_mem[17] = 0x01; g_mem[18] = 0x30; g_mem[19] = 0x00;   // units at 17 = 0x3001 (non-beginning)
    // text "\x0141\x0030": wcslen 2; cursor 17 reads 0x3001 (East Asian but may not begin a line), cursor 19 reads 0x0000
    if (TokenizeW(16, 0, 0, 0) != 1) return fail(__LINE__);

    std::printf("00a10a60_TEST PASS\n");
    return 0;
}