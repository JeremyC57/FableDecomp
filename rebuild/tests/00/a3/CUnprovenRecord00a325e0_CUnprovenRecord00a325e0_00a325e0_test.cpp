#include <cstdio>
#include <new>
// Behaviour model: the string member is constructed and the two following members start cleared.
static int g_ctors;
class CCharString { public: const char* p; CCharString() : p("") { ++g_ctors; } };
struct R { CCharString Name; void* A; void* B; R() : A(0), B(0) { } };
int main()
{
    unsigned char buf[sizeof(R)];
    for (unsigned i = 0; i < sizeof buf; ++i) buf[i] = 0xCD;
    R* r = new (buf) R;
    if (r->A != 0 || r->B != 0 || g_ctors != 1) { std::printf("00a325e0_TEST FAIL\n"); return 1; }
    std::printf("00a325e0_TEST PASS\n");
    return 0;
}