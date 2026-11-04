// Behaviour test for CTCSummonSpell::~CTCSummonSpell (0x0079A620): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_pdef { ~M_pdef() { Log("pdef"); } };
struct M_minion { ~M_minion() { Log("minion"); } };
struct M_removed { ~M_removed() { Log("removed"); } };
struct T : Base {
    M_pdef m_pdef;
    M_minion m_minion;
    M_removed m_removed;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "removed,minion,pdef,base,") == 0;
    printf(ok ? "0079a620_TEST PASS\n" : "0079a620_TEST FAIL\n");
    return ok ? 0 : 1;
}