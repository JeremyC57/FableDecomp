#include <cstdio>
static void* g_this = (void*)1; static long g_a = 0, g_b = 0;
struct O { bool ok; virtual ~O() {} virtual bool Accepts() { return ok; } };
static void SetRange(O* self, long a, long b) { g_this = self; g_a = a; g_b = b; }   // the thiscall setter as a free function: the model records the (possibly null) target
void Func(O* o, long a, long b) { O* t = o->Accepts() ? o : 0; SetRange(t, a, b); }
int main()
{
    O o; o.ok = true; Func(&o, 3, 4);
    if (g_this != &o || g_a != 3 || g_b != 4) { std::printf("00c33f20_TEST FAIL\n"); return 1; }
    o.ok = false; Func(&o, 5, 6);            // the setter receives a null `this` (retail does the same); the model only records it
    if (g_this != 0 || g_a != 5 || g_b != 6) { std::printf("00c33f20_TEST FAIL\n"); return 1; }
    std::printf("00c33f20_TEST PASS\n");
    return 0;
}