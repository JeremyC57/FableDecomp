// Behaviour test for NParticleEngine::CDecal::~CDecal (0x00AF7C10): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_primids { ~M_primids() { Log("primids"); } };
struct M_parents { ~M_parents() { Log("parents"); } };
struct T : Base {
    M_primids m_primids;
    M_parents m_parents;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "parents,primids,base,") == 0;
    printf(ok ? "00af7c10_TEST PASS\n" : "00af7c10_TEST FAIL\n");
    return ok ? 0 : 1;
}