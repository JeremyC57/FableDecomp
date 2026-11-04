// Behaviour test for C3DMeshMaterial::~C3DMeshMaterial (0x00ABF800): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_name { ~M_name() { Log("name"); } };
struct M_filenames { ~M_filenames() { Log("filenames"); } };
struct T : Base {
    M_name m_name;
    M_filenames m_filenames;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "filenames,name,base,") == 0;
    printf(ok ? "00abf800_TEST PASS\n" : "00abf800_TEST FAIL\n");
    return ok ? 0 : 1;
}