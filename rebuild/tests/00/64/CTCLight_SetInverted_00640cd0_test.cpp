#include <cstdio>
// Behaviour model of the CTCLight setter: only an overridden light takes the value and refreshes its thing.
struct Info { float v; bool b; };
struct Light { Info* info; bool overridden; int refreshes; };
static void set(Light& l, float value) { if (l.overridden) { l.info->v = value; ++l.refreshes; } }
int main()
{
    Info i = { 1.5f, false };
    Light l = { &i, false, 0 };
    set(l, 7.0f);
    if (i.v != 1.5f || l.refreshes != 0) { std::printf("00640cd0_TEST FAIL\n"); return 1; }
    l.overridden = true; set(l, 7.0f);
    if (i.v != 7.0f || l.refreshes != 1) { std::printf("00640cd0_TEST FAIL\n"); return 1; }
    std::printf("00640cd0_TEST PASS\n");
    return 0;
}