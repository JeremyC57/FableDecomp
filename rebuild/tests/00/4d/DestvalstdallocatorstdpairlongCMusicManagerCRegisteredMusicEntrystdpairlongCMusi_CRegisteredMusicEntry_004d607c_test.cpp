// Behaviour test for 0x004D607C CTCDiceGame::~CTCDiceGame: the compiler-generated
// destructor runs the members' destructors in reverse declaration order, then the CTCBase destructor.
// Portable model (no engine headers): each destructor appends its name to a log.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class CPointer { public: const char* Name; CPointer() : Name("") { } virtual ~CPointer() { Log(Name); } };
class CVector { public: const char* Name; void* Block; CVector() : Name(""), Block(0) { } ~CVector() { if (Block) free(Block); Log(Name); } };
class CTCBase { public: virtual ~CTCBase() { Log("CTCBase"); } };
class CTCDiceGame : public CTCBase {
public:
    long NumberOfPlayers;
    CPointer CurrentPlayer;
};
int main()
{
    CTCDiceGame* object = new CTCDiceGame;
    object->CurrentPlayer.Name = "CurrentPlayer";
    CTCBase* base = object;
    delete base;
    if (strcmp(g_log, "CurrentPlayer;CTCBase;") != 0) { printf("FAIL %s\n", g_log); return 1; }
    printf("004d607c_TEST PASS\n");
    return 0;
}
