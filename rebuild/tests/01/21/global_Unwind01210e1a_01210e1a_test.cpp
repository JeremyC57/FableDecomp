#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_ctor = 0, g_dtor = 0, g_asg = 0; struct Cs { void* d; Cs(const char* t) : d((void*)t) { ++g_ctor; } Cs& operator=(const Cs& o) { d = o.d; ++g_asg; return *this; } ~Cs() { ++g_dtor; } };
struct X { unsigned char p[0x364]; Cs n[16]; void Set(unsigned long i, const char* t) { L l(&g_g); if (i < 16) n[i] = Cs(t); } };
static bool Check() { X* x = (X*)new char[sizeof(X)]; x->Set(3, "abc"); x->Set(16, "zzz"); bool ok = g_ctor == 1 && g_asg == 1 && g_dtor == 1 && g_l == 0; delete[] (char*)x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0aa40_TEST FAIL\n"); return 1; }
    std::printf("00c0aa40_TEST PASS\n");
    return 0;
}