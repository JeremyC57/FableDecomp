// Behaviour test for CTCRegionDisplay::~CTCRegionDisplay (0x00639900): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_name { ~M_name() { Log("name"); } };
struct M_extra { ~M_extra() { Log("extra"); } };
struct T : Base {
    M_name m_name;
    M_extra m_extra;
    ~T() { Log("remove"); }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "remove,extra,name,base,") == 0;
    printf(ok ? "00639900_TEST PASS\n" : "00639900_TEST FAIL\n");
    return ok ? 0 : 1;
}