// Behaviour test for CTCSummonableCreature::~CTCSummonableCreature (0x0079C200): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_pdef { ~M_pdef() { Log("pdef"); } };
struct M_summoner { ~M_summoner() { Log("summoner"); } };
struct T : Base {
    M_pdef m_pdef;
    M_summoner m_summoner;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "summoner,pdef,base,") == 0;
    printf(ok ? "0079c200_TEST PASS\n" : "0079c200_TEST FAIL\n");
    return ok ? 0 : 1;
}