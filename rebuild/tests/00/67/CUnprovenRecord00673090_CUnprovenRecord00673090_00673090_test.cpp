#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
#include <cstdio>
#include <new>
// Behaviour model: the string member is constructed (empty) and the scalar member starts at zero.
static int g_ctors;
class CCharString { public: const char* p; CCharString() : p("") { ++g_ctors; } };
struct R { CCharString Name; long Value; R() : Value(0) {} };
int main()
{
    unsigned char buf[sizeof(R)];
    for (unsigned i = 0; i < sizeof buf; ++i) buf[i] = 0xAB;
    R* r = new (buf) R;
    if (r->Value != 0 || g_ctors != 1 || r->Name.p[0] != 0) { std::printf("00673090_TEST FAIL\n"); return 1; }
    std::printf("00673090_TEST PASS\n");
    return 0;
}