// Behaviour test for 0x009F1D20 InitializeMemorySystem (the boot.ini line scanner): the same scan over an in-memory
// buffer, recording each name / value pair it finds (retail computes them and drops them).
#include <cstdio>
#include <cctype>
#include <cstring>

static int g_pairs = 0;
static char g_names[8][32];
static char g_values[8][32];

static char* FindChar(char* p, char* end, char ch)
{
    while (p < end && *p != ch)
        ++p;
    return p;
}

static void Scan(char* buffer, unsigned long size)
{
    char* end = buffer + size;
    char* p = buffer;
    while (p < end) {
        while (p < end && std::isspace(*p))
            ++p;
        char* lineEnd = p;
        while (lineEnd < end && *lineEnd != '\n' && *lineEnd != '\r')
            ++lineEnd;
        if (!(p + 1 < end && p[0] == '/' && p[1] == '/')) {
            char* last = lineEnd;
            while (p < last && std::isspace(last[-1]))
                --last;
            char* equals = FindChar(p, last, '=');
            if (p < equals && equals < last) {
                char* nameEnd = equals;
                while (p < nameEnd && std::isspace(nameEnd[-1]))
                    --nameEnd;
                char* value = equals + 1;
                while (value < last && std::isspace(*value))
                    ++value;
                if (g_pairs < 8) {
                    std::memcpy(g_names[g_pairs], p, nameEnd - p); g_names[g_pairs][nameEnd - p] = 0;
                    std::memcpy(g_values[g_pairs], value, last - value); g_values[g_pairs][last - value] = 0;
                }
                ++g_pairs;
            }
        }
        p = lineEnd;
    }
}

int main()
{
    int ok = 1;
    char text[] = "  heap = 64 \r\n// comment = no\r\n=novalue\nnoequals\nkey=\n\tpool\t=\t 12\n x=y";
    Scan(text, (unsigned long)std::strlen(text));
    if (g_pairs != 4) ok = 0;                              // "=novalue" and "noequals" are skipped
    if (std::strcmp(g_names[0], "heap") || std::strcmp(g_values[0], "64")) ok = 0;
    if (std::strcmp(g_names[1], "key") || std::strcmp(g_values[1], "")) ok = 0;
    if (std::strcmp(g_names[2], "pool") || std::strcmp(g_values[2], "12")) ok = 0;
    if (std::strcmp(g_names[3], "x") || std::strcmp(g_values[3], "y")) ok = 0;
    g_pairs = 0;
    Scan(text, 0);
    if (g_pairs != 0) ok = 0;
    if (!ok) { std::printf("009f1d20_TEST FAIL\n"); return 1; }
    std::printf("009f1d20_TEST PASS\n");
    return 0;
}