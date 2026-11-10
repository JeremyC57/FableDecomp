// Behaviour test for 0x008E8CB0 CCreatureAction_BriarRoseInvulnerableTeleport::~CCreatureAction_BriarRoseInvulnerableTeleport: the compiler-generated
// destructor runs the members' destructors in reverse declaration order, then the CCombatActionBase destructor.
// Portable model (no engine headers): each destructor appends its name to a log.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CPointer { public: const char* Name; CPointer() : Name("") { } virtual ~CPointer() { Log(Name); } };
class CVector { public: const char* Name; void* Block; CVector() : Name(""), Block(0) { } ~CVector() { if (Block) free(Block); Log(Name); } };
class CCombatActionBase { public: virtual ~CCombatActionBase() { Log("CCombatActionBase"); } };
class CCreatureAction_BriarRoseInvulnerableTeleport : public CCombatActionBase {
public:
    CPointer PEnemy;
};
int main()
{
    CCreatureAction_BriarRoseInvulnerableTeleport* object = new CCreatureAction_BriarRoseInvulnerableTeleport;
    object->PEnemy.Name = "PEnemy";
    CCombatActionBase* base = object;
    delete base;
    if (strcmp(g_log, "PEnemy;CCombatActionBase;") != 0) { printf("FAIL %s\n", g_log); return 1; }
    printf("008e8cb0_TEST PASS\n");
    return 0;
}
