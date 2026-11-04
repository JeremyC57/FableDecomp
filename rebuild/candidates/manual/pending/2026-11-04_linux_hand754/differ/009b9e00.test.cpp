// Behaviour test for CStringParser::SkipPastNextItemIfIs (0x009B9E00): a model of the row.
#include <stdio.h>

struct Parser { const char* s; long pos; };
static bool Skip(Parser& p, const char* item)
{
    long saved = p.pos;
    while (p.s[p.pos] == ' ') ++p.pos;
    long i = 0; while (item[i] && item[i] == p.s[p.pos + i]) ++i;
    if (!item[i]) { p.pos += i; return true; }
    p.pos = saved; return false;
}
static int run()
{
    Parser p = { "  foo bar", 0 };
    int ok = Skip(p, "foo") && p.pos == 5;
    ok = ok && !Skip(p, "baz") && p.pos == 5 && Skip(p, "bar") && p.pos == 9;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "009b9e00_TEST PASS\n" : "009b9e00_TEST FAIL\n");
    return ok ? 0 : 1;
}
