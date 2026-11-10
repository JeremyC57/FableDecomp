// Behaviour test for 0x007EB470 CCreatureAction_AddRealObjectToInventory::~CCreatureAction_AddRealObjectToInventory: the compiler-generated
// destructor runs the members' destructors in reverse declaration order, then the CCreatureActionBase destructor.
// Portable model (no engine headers): each destructor appends its name to a log.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CPointer { public: const char* Name; CPointer() : Name("") { } virtual ~CPointer() { Log(Name); } };
class CVector { public: const char* Name; void* Block; CVector() : Name(""), Block(0) { } ~CVector() { if (Block) free(Block); Log(Name); } };
class CCreatureActionBase { public: virtual ~CCreatureActionBase() { Log("CCreatureActionBase"); } };
class CCreatureAction_AddRealObjectToInventory : public CCreatureActionBase {
public:
    CPointer PReceivedObject;
};
int main()
{
    CCreatureAction_AddRealObjectToInventory* object = new CCreatureAction_AddRealObjectToInventory;
    object->PReceivedObject.Name = "PReceivedObject";
    CCreatureActionBase* base = object;
    delete base;
    if (strcmp(g_log, "PReceivedObject;CCreatureActionBase;") != 0) { printf("FAIL %s\n", g_log); return 1; }
    printf("007eb470_TEST PASS\n");
    return 0;
}
