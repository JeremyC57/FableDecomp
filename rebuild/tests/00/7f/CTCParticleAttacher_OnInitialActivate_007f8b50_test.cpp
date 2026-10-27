// Behaviour test for 0x007F8B50: OnInitialActivate attaches particles only when none are attached
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SAttacher { int Begin, End; int Attaches;
    bool empty() const { return Begin == End; }
    void AttachParticles() { ++Attaches; End = Begin + 3; }
    void OnInitialActivate() { if (empty()) AttachParticles(); } };
int main()
{
    SAttacher a; a.Begin = a.End = 0; a.Attaches = 0;
    a.OnInitialActivate(); CHECK(a.Attaches == 1);
    a.OnInitialActivate(); CHECK(a.Attaches == 1);
    if (!g_fail) printf("007f8b50_TEST PASS\n");
    return g_fail;
}