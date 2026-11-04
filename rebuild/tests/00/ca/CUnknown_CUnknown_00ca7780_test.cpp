// Behaviour test for CUnknown::~CUnknown (0x00CA7780): destroying a CUnknown runs the CBaseObject base destructor, which
// drops the live-object count.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static long g_cObjects = 2;
struct BaseObject { ~BaseObject() { --g_cObjects; } };
struct NDUnknown { virtual long NDQI() { return 0; } };
struct Unknown : NDUnknown, BaseObject { void* m_pUnknown; long m_cRef; virtual ~Unknown() {} };
int main()
{
    { Unknown u; u.m_pUnknown = 0; u.m_cRef = 0; }
    bool ok = g_cObjects == 1;
    printf(ok ? "00ca7780_TEST PASS\n" : "00ca7780_TEST FAIL\n");
    return ok ? 0 : 1;
}