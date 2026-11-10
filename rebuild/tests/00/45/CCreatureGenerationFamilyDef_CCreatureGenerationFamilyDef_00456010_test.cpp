// Behaviour test for 0x00456010 CCreatureGenerationFamilyDef::~CCreatureGenerationFamilyDef: the compiler-generated
// destructor runs the members' destructors in reverse declaration order, then the CParentDefClassBase destructor.
// Portable model (no engine headers): each destructor appends its name to a log.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CPointer { public: const char* Name; CPointer() : Name("") { } virtual ~CPointer() { Log(Name); } };
class CVector { public: const char* Name; void* Block; CVector() : Name(""), Block(0) { } ~CVector() { if (Block) free(Block); Log(Name); } };
class CParentDefClassBase { public: virtual ~CParentDefClassBase() { Log("CParentDefClassBase"); } };
class CCreatureGenerationFamilyDef : public CParentDefClassBase {
public:
    CVector Creatures;
    long DifficultyLevel;
    long GeneratorObject;
    CVector GenerationTypes;
};
int main()
{
    CCreatureGenerationFamilyDef* object = new CCreatureGenerationFamilyDef;
    object->Creatures.Name = "Creatures";
    object->Creatures.Block = malloc(16);
    object->GenerationTypes.Name = "GenerationTypes";
    object->GenerationTypes.Block = malloc(16);
    CParentDefClassBase* base = object;
    delete base;
    if (strcmp(g_log, "GenerationTypes;Creatures;CParentDefClassBase;") != 0) { printf("FAIL %s\n", g_log); return 1; }
    printf("00456010_TEST PASS\n");
    return 0;
}
