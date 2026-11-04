// Behaviour test for CTCForcePushPower::~CTCForcePushPower (0x007A5460): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_pdef { ~M_pdef() { Log("pdef"); } };
struct M_removed { ~M_removed() { Log("removed"); } };
struct T : Base {
    M_pdef m_pdef;
    M_removed m_removed;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "removed,pdef,base,") == 0;
    printf(ok ? "007a5460_TEST PASS\n" : "007a5460_TEST FAIL\n");
    return ok ? 0 : 1;
}