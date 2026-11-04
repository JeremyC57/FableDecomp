// Behaviour test for CTCActionUseBuyHouse::~CTCActionUseBuyHouse (0x0072E6C0): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_usedef { ~M_usedef() { Log("usedef"); } };
struct M_pdef { ~M_pdef() { Log("pdef"); } };
struct T : Base {
    M_usedef m_usedef;
    M_pdef m_pdef;
    ~T() { Log("remove"); }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "remove,pdef,usedef,base,") == 0;
    printf(ok ? "0072e6c0_TEST PASS\n" : "0072e6c0_TEST FAIL\n");
    return ok ? 0 : 1;
}