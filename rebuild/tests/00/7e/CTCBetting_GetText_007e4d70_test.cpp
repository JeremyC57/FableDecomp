// behaviour test for 0x007E4D70: a found entry gives its text, a missing one the fallback; the entry reference is released
#include <cstdio>
#include <cstring>
struct Entry { int refs; const char* text; };
static Entry g_entry = { 1, "hello" };
static const char* GetText(unsigned long id)
{
    static const char* missing = "Missing text entry";
    Entry* e = id == 7 ? &g_entry : 0; if (e) ++e->refs;
    const char* r = e ? e->text : missing;
    if (e) --e->refs;
    return r;
}
int main()
{
    if (std::strcmp(GetText(7), "hello") || std::strcmp(GetText(3), "Missing text entry") || g_entry.refs != 1) { std::printf("007e4d70_TEST FAIL\n"); return 1; }
    std::printf("007e4d70_TEST PASS\n");
    return 0;
}