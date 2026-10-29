// Behaviour test for NScript::CQ_FireHeartScript::CPuzzleBlockMoon::Main 0x00D37D00: wait for Me's data string, take it as
// the pad number, wait for the hero to be 1.5 pad radii away, then per frame until the mission succeeds: on a block reset
// turn into a sun sign when the starting pattern marks the pad, count the reset, wait out the reset and the
// hero's distance; otherwise, hero within the pad radius outside the round edges -> play the lock sound and turn.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Parent {
    bool MissionSucceeded, ResetBlocks, RoundEnded, RoundBeginning;
    long NumBlocksReset;
    float PadRadius;
    bool StartingPattern[8];
};
struct Block;
struct World {                       // a scripted timeline: one step per NewScriptFrame
    Block* block;
    int frame;
    void NewScriptFrame();
};
struct Block {
    World* Game;
    Parent* ParentClass;
    long PadNumber;
    const char* data;                // Me's data string
    float heroDistance;
    int terminateAt, calls;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    bool DataIsNull() const { return std::strcmp(data, "NULL") == 0; }
    void TurnIntoASunSign(bool hero) { Log(hero ? "Sun(1)" : "Sun(0)"); }
    void PlaySoundOnThing(const char* s) { Log(s); }
    bool Over(float d) const { return heroDistance > d; }
    bool Under(float d) const { return heroDistance < d; }
    void Main()
    {
        while (!(!DataIsNull())) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        PadNumber = 0; for (const char* p = data; *p; ++p) PadNumber = PadNumber * 10 + (*p - '0');
        while (!Over((float)(ParentClass->PadRadius * 1.5))) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (!ParentClass->MissionSucceeded) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (ParentClass->ResetBlocks) {
                if (IsActiveThreadTerminating()) return;
                if (ParentClass->StartingPattern[PadNumber]) {
                    if (IsActiveThreadTerminating()) return;
                    TurnIntoASunSign(false);
                }
                ParentClass->NumBlocksReset++;
                while (ParentClass->ResetBlocks) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                while (!Over((float)(ParentClass->PadRadius * 1.5))) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            else if (Under(ParentClass->PadRadius) && !ParentClass->RoundEnded && !ParentClass->RoundBeginning) {
                if (IsActiveThreadTerminating()) return;
                PlaySoundOnThing("SND_COMBAT_MULITIPLIERLOCK_02");
                TurnIntoASunSign(true);
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
// timeline: frame 1 data set to "3"; hero stays close (1.0) until frame 3 then 20.0 away; frame 5 reset starts,
// frame 7 reset ends; frame 9 hero steps on the pad (1.0) during a round; frame 11 round ended (no trigger);
// frame 12 mission succeeds.
void World::NewScriptFrame()
{
    ++frame;
    Block* b = block; Parent* p = b->ParentClass;
    if (frame == 1) b->data = "3";
    if (frame == 3) b->heroDistance = 20.0f;
    if (frame == 5) p->ResetBlocks = true;
    if (frame == 7) p->ResetBlocks = false;
    if (frame == 9) b->heroDistance = 1.0f;
    if (frame == 10) { b->heroDistance = 20.0f; p->RoundEnded = true; }
    if (frame == 11) b->heroDistance = 1.0f;
    if (frame == 12) p->MissionSucceeded = true;
}
static bool Run(bool patternBit, int terminateAt, const char* expect, long expectResets)
{
    g_log[0] = 0;
    Parent p; std::memset(&p, 0, sizeof(p)); p.PadRadius = 4.0f; p.StartingPattern[3] = patternBit;
    World w; Block b;
    w.block = &b; w.frame = 0;
    b.Game = &w; b.ParentClass = &p; b.PadNumber = -1; b.data = "NULL"; b.heroDistance = 1.0f; b.terminateAt = terminateAt; b.calls = 0;
    b.Main();
    if (std::strcmp(g_log, expect) != 0 || p.NumBlocksReset != expectResets) { std::printf("got %s %ld\n", g_log, p.NumBlocksReset); return false; }
    return terminateAt != 0 || b.PadNumber == 3;
}
int main()
{
    bool ok = Run(true, 0, "Sun(0);SND_COMBAT_MULITIPLIERLOCK_02;Sun(1);", 1)
           && Run(false, 0, "SND_COMBAT_MULITIPLIERLOCK_02;Sun(1);", 1)
           && Run(true, 2, "", 0);
    if (!ok) { std::printf("00d37d00_TEST FAIL\n"); return 1; }
    std::printf("00d37d00_TEST PASS\n");
    return 0;
}