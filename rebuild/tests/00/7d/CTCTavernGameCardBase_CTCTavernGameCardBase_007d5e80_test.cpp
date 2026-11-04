// Behaviour test for CTCTavernGameCardBase::~CTCTavernGameCardBase (0x007D5E80): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_cards { ~M_cards() { Log("cards"); } };
struct M_pack { ~M_pack() { Log("pack"); } };
struct M_particles { ~M_particles() { Log("particles"); } };
struct T : Base {
    M_cards m_cards;
    M_pack m_pack;
    M_particles m_particles;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "particles,pack,cards,base,") == 0;
    printf(ok ? "007d5e80_TEST PASS\n" : "007d5e80_TEST FAIL\n");
    return ok ? 0 : 1;
}