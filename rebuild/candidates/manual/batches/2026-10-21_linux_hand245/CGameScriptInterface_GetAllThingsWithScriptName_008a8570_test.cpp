// Behaviour test for 008a8570 CGameScriptInterface::GetAllThingsWithScriptName: append one script thing per match, return the output size.
#include <cstdio>
#include <cstring>
struct Thing { const char* name; int id; };
struct Out { int ids[16]; long n; };
static long GetAll(const char* name, Thing* world, int count, Out& out)
{
    Thing* found[16]; int nf = 0;
    for (int i = 0; i < count; ++i) if (!std::strcmp(world[i].name, name)) found[nf++] = &world[i];
    if (nf == 0) return 0;
    for (Thing** it = found; it != found + nf; ++it) { Thing* t = *it; out.ids[out.n++] = t->id; }
    return out.n;
}
int main()
{
    Thing w[4] = { { "A", 1 }, { "B", 2 }, { "A", 3 }, { "C", 4 } };
    Out o; o.n = 1; o.ids[0] = 99;
    long r = GetAll("A", w, 4, o);
    long r0 = GetAll("Z", w, 4, o);
    if (r != 3 || o.ids[1] != 1 || o.ids[2] != 3 || r0 != 0 || o.n != 3) { std::printf("008a8570_TEST FAIL\n"); return 1; }
    std::printf("008a8570_TEST PASS\n");
    return 0;
}
