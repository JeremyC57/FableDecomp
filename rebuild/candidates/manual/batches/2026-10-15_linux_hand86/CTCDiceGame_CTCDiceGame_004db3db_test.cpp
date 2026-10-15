// Behaviour test for CTCDiceGame::CTCDiceGame(CThing*) @ 0x004DB3DB: NumberOfPlayers 0, CurrentPlayer built from a null thing.
#include <cstdio>
#include <new>
struct CThing { int id; };
static CThing* g_baseParent = 0;
static int g_ptrCtors = 0;
static CThing* g_ptrArg = (CThing*)1;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
struct CIntelligentPointer_CThing {
    void* __vftable;
    void* PData;
    CIntelligentPointer_CThing(CThing* thing) : __vftable(0), PData(thing) { ++g_ptrCtors; g_ptrArg = thing; }
};
struct CTCDiceGame : CTCBase_Calls {
    long                       NumberOfPlayers;
    CIntelligentPointer_CThing CurrentPlayer;
    virtual void v00() { }
    CTCDiceGame(CThing* parent);
};
CTCDiceGame::CTCDiceGame(CThing* parent) : CTCBase_Calls(parent), NumberOfPlayers(0), CurrentPlayer(0) { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCDiceGame)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCDiceGame* g = new (u.raw) CTCDiceGame(&thing);
    if (g_baseParent != &thing || g->NumberOfPlayers != 0 || g_ptrCtors != 1 || g_ptrArg != 0 || g->CurrentPlayer.PData != 0)
        { std::printf("004db3db_TEST FAIL\n"); return 1; }
    std::printf("004db3db_TEST PASS\n");
    return 0;
}
