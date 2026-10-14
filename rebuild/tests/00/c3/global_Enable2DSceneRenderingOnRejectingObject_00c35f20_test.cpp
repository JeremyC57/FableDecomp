#include <cstdio>
static void* g_target = (void*)1; static long g_a = 0, g_b = 0; static int g_calls = 0;
struct CObj { virtual void v00() { } virtual bool Accepts() { return accepts; } bool accepts;
    void Set(long v) { ++g_calls; g_target = this; g_a = v; } void Set2(long v) { ++g_calls; g_target = this; g_b = v; } };
static void Call(CObj* object, long a) { CObj* t = object->Accepts() ? 0 : object; t->Set(a);  }
int main() { CObj o; o.accepts = false; Call(&o, 7); if (g_target != &o || g_a != 7  || g_calls != 1) { std::printf("00c35f20_TEST FAIL\n"); return 1; }
    // the rejecting case would dereference null in retail: only the accepting arm is exercised
    std::printf("00c35f20_TEST PASS\n"); return 0; }