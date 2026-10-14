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
    M(bool a) : name(), bs(), hp(), dob(), mv(), mg(), anim(a) {}
};
int main()
{
    M m(true); M n(false);
    if (sizeof(M) != 0x7C || g_s != 2 || m.name.p || m.bs.r != 0 || m.hp.el || m.dob.n || m.mv.d || m.mg.e || !m.anim || n.anim) { std::printf("00a2f270_TEST FAIL\n"); return 1; }
    std::printf("00a2f270_TEST PASS\n");
    return 0;
}