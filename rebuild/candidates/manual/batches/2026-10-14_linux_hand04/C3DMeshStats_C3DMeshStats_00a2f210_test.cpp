#include <cstdio>
static int g_s = 0;
struct Str { char* p; Str() : p(0) { ++g_s; } };
struct BS { float c[3]; float r; BS() { c[0] = 0; c[1] = 0; c[2] = 0; r = 0; } };
struct PM { char cmp; void* el; unsigned long n; bool d; PM() : el(0), n(0), d(false) {} };
struct VM { void* f; void* l; void* e; char cmp; bool d; VM() : f(0), l(0), e(0), d(false) {} };
struct AR { void* f; void* l; void* e; AR() : f(0), l(0), e(0) {} };
struct M {
    virtual void v00() {}
    void* pool; long h; Str name; BS bs; float bb[6]; PM hp, dob; VM mv; AR mg; char* pn; unsigned short pns; bool anim;
    M() : pool(0), h(-1), name(), bs(), hp(), dob(), mv(), mg(), pn(0), pns(0), anim(false) {}
};
int main()
{
    M m;
    if (sizeof(M) != 0x7C || g_s != 1 || m.pool || m.h != -1 || m.name.p || m.bs.r != 0 || m.bs.c[2] != 0 || m.hp.el || m.hp.n || m.hp.d ||
        m.dob.el || m.mv.f || m.mv.e || m.mv.d || m.mg.l || m.pn || m.pns || m.anim) { std::printf("00a2f210_TEST FAIL\n"); return 1; }
    std::printf("00a2f210_TEST PASS\n");
    return 0;
}
