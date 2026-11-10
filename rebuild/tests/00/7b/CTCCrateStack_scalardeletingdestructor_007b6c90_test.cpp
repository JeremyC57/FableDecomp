// Behaviour test for 0x007B6C90 CTCCrateStack::`scalar deleting destructor: the compiler-generated
// destructor runs the members' destructors in reverse declaration order, then the CTCBase destructor (then the block is freed).
// Portable model (no engine headers): each destructor appends its name to a log.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CPointer { public: const char* Name; CPointer() : Name("") { } virtual ~CPointer() { Log(Name); } };
class CVector { public: const char* Name; void* Block; CVector() : Name(""), Block(0) { } ~CVector() { if (Block) free(Block); Log(Name); } };
class CTCBase { public: virtual ~CTCBase() { Log("CTCBase"); } };
class CTCCrateStack : public CTCBase {
public:
    CPointer FirstCrate;
    CPointer SecondCrate;
    long CrateDefIndex;
};
int main()
{
    CTCCrateStack* object = new CTCCrateStack;
    object->FirstCrate.Name = "FirstCrate";
    object->SecondCrate.Name = "SecondCrate";
    CTCBase* base = object;
    delete base;
    if (strcmp(g_log, "SecondCrate;FirstCrate;CTCBase;") != 0) { printf("FAIL %s\n", g_log); return 1; }
    printf("007b6c90_TEST PASS\n");
    return 0;
}
