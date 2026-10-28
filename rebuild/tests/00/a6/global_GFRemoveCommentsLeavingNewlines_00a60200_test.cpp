#include <cstdio>
#include <cstring>

// Test copy of GFRemoveCommentsLeavingNewlines: cut each commentStart..commentEnd block, keep its '\n' / '\r'.
// Retail quirk kept: the tail memmove moves textLength - returns - newlines - end + 1 bytes, i.e. newlines + returns
// bytes short, so a cut block that held line breaks leaves that many stale bytes of the old text (and the old
// terminator) behind the moved tail.
static void RemoveCommentsLeavingNewlines(char* text, const char* commentStart, const char* commentEnd)
{
    unsigned long textLength = std::strlen(text);
    unsigned long startLength = std::strlen(commentStart);
    unsigned long endLength = std::strlen(commentEnd);
    for (unsigned long i = 0; i < textLength; ++i) {
        if (std::strncmp(commentStart, text + i, startLength) == 0) {
            for (unsigned long j = i + startLength; j < textLength; ++j) {
                if (std::strncmp(text + j, commentEnd, endLength) == 0) {
                    unsigned long end = j + endLength;
                    unsigned long newlines = 0;
                    unsigned long returns = 0;
                    unsigned long k;
                    for (k = i; k < end; ++k) {
                        if (text[k] == '\n')
                            ++newlines;
                        if (text[k] == '\r')
                            ++returns;
                    }
                    for (k = i; k < i + newlines; ++k)
                        text[k] = '\n';
                    for (k = i + newlines; k < i + newlines + returns; ++k)
                        text[k] = '\r';
                    std::memmove(text + i + newlines + returns, text + end, textLength - returns - newlines - end + 1);
                    break;
                }
            }
        }
    }
}

int main()
{
    int ok = 1;
    char a[64];
    std::strcpy(a, "x = 1; /* one\r\ntwo\r\n */ y = 2;");
    RemoveCommentsLeavingNewlines(a, "/*", "*/");
    if (std::strcmp(a, "x = 1; \n\n\r\r y =two\r\n */ y = 2;") != 0) ok = 0;
    char b[64];
    std::strcpy(b, "a/*b*/c/*d*/e");
    RemoveCommentsLeavingNewlines(b, "/*", "*/");
    if (std::strcmp(b, "ace") != 0) ok = 0;
    char c[64];
    std::strcpy(c, "keep /* unterminated");
    RemoveCommentsLeavingNewlines(c, "/*", "*/");
    if (std::strcmp(c, "keep /* unterminated") != 0) ok = 0;
    char d[64];
    std::strcpy(d, "p//q\nr");
    RemoveCommentsLeavingNewlines(d, "//", "\n");
    if (std::strcmp(d, "p\nrq\nr") != 0) ok = 0;
    if (!ok) { std::printf("00a60200_TEST FAIL\n"); return 1; }
    std::printf("00a60200_TEST PASS\n");
    return 0;
}