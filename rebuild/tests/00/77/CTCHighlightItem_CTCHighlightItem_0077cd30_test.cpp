// Behaviour test for CTCHighlightItem::~CTCHighlightItem (0x0077CD30): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct EmitterMember { ~EmitterMember() { Log("emitter"); } };
struct T : Base {
    EmitterMember emitterMember;
    ~T() { Log("stop"); }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "stop,emitter,base,") == 0;
    printf(ok ? "0077cd30_TEST PASS\n" : "0077cd30_TEST FAIL\n");
    return ok ? 0 : 1;
}