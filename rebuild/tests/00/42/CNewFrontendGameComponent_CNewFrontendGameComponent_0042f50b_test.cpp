#include <cstdio>
static int g_log[16]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Member { int id; ~Member() { Log(id); } };
struct Def { long m_count; void Destroy() { Log(99); } };
struct DefPtr { Def* Data; ~DefPtr() { Def* d = Data; if (d) { if (--d->m_count == 0) d->Destroy(); Data = 0; } } };
struct Base { bool m_bShutDown; virtual ~Base() { Log(100); } };
struct Comp : Base {
    Member font; Member init; DefPtr def; Member bank; Member save;
    int shutdowns;
    void Shutdown() { ++shutdowns; Log(50); }
    virtual ~Comp() { if (!m_bShutDown) Shutdown(); }
};
int main()
{
    Def d; d.m_count = 1;
    {
        Comp* c = new Comp; c->m_bShutDown = false; c->shutdowns = 0;
        c->font.id = 1; c->init.id = 2; c->def.Data = &d; c->bank.id = 3; c->save.id = 4;
        delete c;
    }
    int expect[] = { 50, 4, 3, 99, 2, 1, 100 };
    bool fail = g_n != 7;
    for (int i = 0; i < 7 && !fail; ++i) if (g_log[i] != expect[i]) fail = true;
    if (d.m_count != 0) fail = true;
    if (fail) { std::printf("0042f50b_TEST FAIL\n"); return 1; }
    std::printf("0042f50b_TEST PASS\n");
    return 0;
}