// Behaviour test for NParticleEngine::CParticleUpdateParameters<ulong,ulong>::~CParticleUpdateParameters (0x006E2500): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_vector { ~M_vector() { Log("vector"); } };
struct M_update { ~M_update() { Log("update"); } };
struct T : Base {
    M_vector m_vector;
    M_update m_update;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "update,vector,base,") == 0;
    printf(ok ? "006e2500_TEST PASS\n" : "006e2500_TEST FAIL\n");
    return ok ? 0 : 1;
}