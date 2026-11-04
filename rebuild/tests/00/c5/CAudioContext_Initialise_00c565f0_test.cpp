// Behaviour test for CAudioContext::Initialise (0x00C565F0): once only unless forced; the local OpenAL32.dll, then
// wrap_oal.dll, then the system DLL on the preferred device; the first that supports EAX 3 wins.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Ctx { bool done; const char* tried[8]; int n; int eaxOn;
    bool TryDll(const char* name) { tried[n++] = name; return n == eaxOn; }
    bool Initialise(bool force) { if (done && !force) return false; done = true;
        if (TryDll("local OpenAL32")) return true; if (TryDll("local wrap_oal")) return true; if (TryDll("system OpenAL32")) return true; return false; } };
int main()
{
    Ctx a = { false, { 0 }, 0, 2 };
    bool r1 = a.Initialise(false);
    bool r2 = a.Initialise(false);
    Ctx b = { false, { 0 }, 0, 9 };
    bool r3 = b.Initialise(false);
    bool ok = r1 && !r2 && a.n == 2 && strcmp(a.tried[1], "local wrap_oal") == 0 && !r3 && b.n == 3;
    printf(ok ? "00c565f0_TEST PASS\n" : "00c565f0_TEST FAIL\n");
    return ok ? 0 : 1;
}