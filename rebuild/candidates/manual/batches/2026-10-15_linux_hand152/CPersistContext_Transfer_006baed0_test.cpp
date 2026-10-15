#include <cstdio>
static int g_ctor = 0, g_copy = 0, g_dtor = 0, g_seen = -1;
struct Rec {
    int tag;
    Rec() : tag(42) { ++g_ctor; }
    Rec(const Rec& o) : tag(o.tag) { ++g_copy; }
    ~Rec() { ++g_dtor; }
};
template <class T> struct Default { static T Get() { T value; return value; } };
struct Ctx {
    void Transfer(const char*, Rec& value, const Rec& def) { g_seen = def.tag; value.tag = def.tag + 1; }
    void Transfer(const char* name, Rec& value) { Rec def = Default<Rec>::Get(); Transfer(name, value, def); }
};
int main()
{
    Ctx c; Rec v; v.tag = 0; g_ctor = g_dtor = 0;
    c.Transfer("x", v);
    if (g_seen != 42 || v.tag != 43 || g_ctor != 1 || g_dtor != g_ctor + g_copy) { std::printf("006baed0_TEST FAIL\n"); return 1; }
    std::printf("006baed0_TEST PASS\n");
    return 0;
}
