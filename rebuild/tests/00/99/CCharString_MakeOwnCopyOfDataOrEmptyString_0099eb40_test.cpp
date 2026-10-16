#include <cstdio>
#include <cstring>
struct Data { char* text; long refs; long reserve; };
static int g_unassign = 0;
struct Str {
    Data* p;
    void Unassign() { ++g_unassign; if (p) --p->refs; }
    void MakeOwnCopyOrEmpty()
    {
        Data* d = p;
        if (d != 0) {
            if (d->refs > 1) {
                Data* c = new Data; c->text = d->text; c->refs = 1; c->reserve = 0;
                Unassign();
                p = c;
            }
        } else {
            Data* e = new Data; e->text = 0; e->refs = 1; e->reserve = 0x20;
            p = e;
        }
    }
};
int main()
{
    char buf[] = "abc";
    Data shared = { buf, 2, 0 };
    Data solo = { buf, 1, 0 };
    Str a; a.p = &shared; a.MakeOwnCopyOrEmpty();
    Str b; b.p = &solo; b.MakeOwnCopyOrEmpty();
    Str c; c.p = 0; c.MakeOwnCopyOrEmpty();
    bool ok = a.p != &shared && a.p->refs == 1 && shared.refs == 1 && std::strcmp(a.p->text, "abc") == 0
           && b.p == &solo && g_unassign == 1 && c.p && c.p->refs == 1 && c.p->reserve == 0x20;
    delete a.p; delete c.p;
    if (!ok) { std::printf("0099eb40_TEST FAIL\n"); return 1; }
    std::printf("0099eb40_TEST PASS\n");
    return 0;
}