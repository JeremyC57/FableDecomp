// Behaviour test for CTCWillEnflame::~CTCWillEnflame (0x007A4310): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_pdef { ~M_pdef() { Log("pdef"); } };
struct M_removed { ~M_removed() { Log("removed"); } };
struct M_target { ~M_target() { Log("target"); } };
struct T : Base {
    M_pdef m_pdef;
    M_removed m_removed;
    M_target m_target;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "target,removed,pdef,base,") == 0;
    printf(ok ? "007a4310_TEST PASS\n" : "007a4310_TEST FAIL\n");
    return ok ? 0 : 1;
}