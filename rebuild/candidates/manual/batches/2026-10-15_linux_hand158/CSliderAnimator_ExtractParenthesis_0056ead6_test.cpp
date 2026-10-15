// Behaviour test for 0x0056EAD6: substring from the first '(' to the end, or empty.
#include <cstdio>
#include <cstring>
static void Extract(const char* text, char* out)
{
    const char* p = std::strchr(text, '(');
    if (p) { std::strcpy(out, p); return; }
    out[0] = 0;
}
int main()
{
    char out[64];
    Extract("Volume (x2)", out); bool ok = std::strcmp(out, "(x2)") == 0;
    Extract("none", out); ok = ok && out[0] == 0;
    Extract("(a)(b)", out); ok = ok && std::strcmp(out, "(a)(b)") == 0;
    if (!ok) { std::printf("0056ead6_TEST FAIL\n"); return 1; }
    std::printf("0056ead6_TEST PASS\n");
    return 0;
}
