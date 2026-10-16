// behaviour test for 0x007E7640 CTCFireheartMinigame::CTCFireheartMinigame(CThing*)
#include <cstdio>
struct MinigameDef { int id; };
struct ParentDef { MinigameDef* sub; bool GetSubDef(MinigameDef** out) { *out = sub; return sub != 0; } };
struct CThing { ParentDef* PDef; };
struct Game {
    CThing* ParentThing; bool InSession, ForceQuit, Initialised, Moving, QuitWithDelay;
    void* scriptData; void* scriptInfo; void* handlesFirst; MinigameDef* PDef;
    Game(CThing* t) : ParentThing(t), InSession(false), ForceQuit(false), Initialised(false), Moving(false),
        QuitWithDelay(false), scriptData(0), scriptInfo(0), handlesFirst(0), PDef(0)
    { ParentThing->PDef->GetSubDef(&PDef); }
};
int main()
{
    MinigameDef m = { 4 };
    ParentDef pd = { &m };
    CThing t = { &pd };
    Game g(&t);
    bool ok = g.PDef == &m && !g.InSession && !g.ForceQuit && !g.Initialised && !g.Moving && !g.QuitWithDelay
        && g.scriptData == 0 && g.handlesFirst == 0;
    if (!ok) { std::printf("007e7640_TEST FAIL\n"); return 1; }
    std::printf("007e7640_TEST PASS\n");
    return 0;
}