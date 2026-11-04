// Behaviour test for NCameraModes::CPhysicsCollisionPointsObject::~CPhysicsCollisionPointsObject (0x00850330): the destructor runs the user body, then the members in reverse
// declaration order, then the base (modelled with logging member/base classes).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); strcat(g_log, ","); }
struct Base { ~Base() { Log("base"); } };
struct M_points { ~M_points() { Log("points"); } };
struct M_smart { ~M_smart() { Log("smart"); } };
struct M_tests { ~M_tests() { Log("tests"); } };
struct T : Base {
    M_points m_points;
    M_smart m_smart;
    M_tests m_tests;
    ~T() {  }
};
int main()
{
    { T t; }
    bool ok = strcmp(g_log, "tests,smart,points,base,") == 0;
    printf(ok ? "00850330_TEST PASS\n" : "00850330_TEST FAIL\n");
    return ok ? 0 : 1;
}