// Behaviour test for 0x007B51B0 __copy_aux(first, last, result, __false_type): forwards everything to __copy with a
// random-access tag and a null distance pointer, returning __copy's inserter.
#include <cstdio>
struct Tag {};
struct It { int pos; };
struct Inserter { int* sink; };
static int g_calls = 0;
static int* g_lastDistance = (int*)1;
static Inserter CopyImpl(It first, It last, Inserter result, const Tag&, int* distance)
{
    ++g_calls; g_lastDistance = distance;
    for (int i = first.pos; i < last.pos; ++i) *result.sink += i;
    return result;
}
static Inserter CopyAux(It first, It last, Inserter result)
{
    return CopyImpl(first, last, result, Tag(), (int*)0);
}
int main()
{
    int sum = 0;
    It f = { 2 }, l = { 5 };
    Inserter in = { &sum };
    Inserter out = CopyAux(f, l, in);
    if (g_calls != 1 || g_lastDistance != 0 || out.sink != &sum || sum != 2 + 3 + 4) { std::printf("007b51b0_TEST FAIL\n"); return 1; }
    std::printf("007b51b0_TEST PASS\n");
    return 0;
}