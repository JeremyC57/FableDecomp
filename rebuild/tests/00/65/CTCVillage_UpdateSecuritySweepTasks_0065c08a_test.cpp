// Behaviour test for 0065c08a: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillage::UpdateSecuritySweepTasks() -- while a security sweep runs: a hero who is no longer a criminal ends it; otherwise the sweep is recalculated every 7.5 seconds or when the hero has moved more than 10 units from where it was last calculated
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct C3DVector { float X, Y, Z; };
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer& GetMainPlayer();                   // 0x00449970
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550

struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
struct CTaskList {                              // retail STLport list: the head node pointer
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};

struct CTCVillage {
    void*         __vftable;                    // +0x00
    void*         ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x8];
    long          FrameStartedSecuritySweep;    // +0x10
    C3DVector     PositionPlayerWasInWhenSecuritySweepCalculated;   // +0x14
    unsigned char _pad_0x20[0x3c];
    CTaskList     SecuritySweepTasks;           // +0x5c list<CCountedPointer<CVillageTaskSecuritySweep> >
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    bool IsHeroCriminal();                      // 0x0065BFCE
    void InvalidateAndClearAllSecuritySweepTasks();   // 0x006590A0
    void CalculateSecuritySweepTasks();         // 0x00659800
    void UpdateSecuritySweepTasks();
};

void CTCVillage::UpdateSecuritySweepTasks()
{
    if (!SecuritySweepTasks.empty()) {
        if (!IsHeroCriminal()) {
            InvalidateAndClearAllSecuritySweepTasks();
            return;
        }
        bool recalculate = false;
        CWorld* world = GetWorld();
        long framesPerSecond = g_framesPerSecond;
        if ((world->GetWorldFrame() - FrameStartedSecuritySweep) % (long)(framesPerSecond * 7.5f) == 0)
            recalculate = true;
        CThing* hero = GetPlayerManager()->GetMainPlayer().GetHeroThing();
        if (hero) {
            const C3DVector& position = hero->GetPosition();
            float dx = PositionPlayerWasInWhenSecuritySweepCalculated.X - position.X;
            float dy = PositionPlayerWasInWhenSecuritySweepCalculated.Y - position.Y;
            float distanceSquared = dx * dx;
            distanceSquared += dy * dy;
            if (distanceSquared > 100.0f)
                recalculate = true;
        }
        if (recalculate) {
            InvalidateAndClearAllSecuritySweepTasks();
            CalculateSecuritySweepTasks();
        }
    }
}

// ---- stubs and driver ----
static C3DVector g_heroPos;
const C3DVector& CThing::GetPosition() const { return g_heroPos; }
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
CThing* CPlayer::GetHeroThing() { return g_heroPtr; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static CPlayerManager g_manager;
static long g_frame;
long CWorld::GetWorldFrame() { return g_frame; }
long g_framesPerSecond = 10;
static CWorld g_world;
CWorld* CTCVillage::GetWorld() const { return &g_world; }
CPlayerManager* CTCVillage::GetPlayerManager() { return &g_manager; }
static bool g_criminal = true;
bool CTCVillage::IsHeroCriminal() { return g_criminal; }
static char g_log[16]; static int g_n;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }
void CTCVillage::InvalidateAndClearAllSecuritySweepTasks() { Log('I'); }
void CTCVillage::CalculateSecuritySweepTasks() { Log('C'); }
static bool Run(CTCVillage& v, const char* want)
{
    g_n = 0; g_log[0] = 0;
    v.UpdateSecuritySweepTasks();
    if (std::strcmp(g_log, want) != 0) { std::printf("0065c08a_TEST FAIL (%s want %s)\n", g_log, want); return false; }
    return true;
}

int main()
{
    static SListNode head, task;
    static CTCVillage village;
    head._M_next = &head; village.SecuritySweepTasks._M_node = &head;
    if (!Run(village, "")) return 1;                                   // no sweep running
    head._M_next = &task;
    village.FrameStartedSecuritySweep = 100;
    village.PositionPlayerWasInWhenSecuritySweepCalculated.X = 5; village.PositionPlayerWasInWhenSecuritySweepCalculated.Y = 5;
    g_heroPos.X = 8; g_heroPos.Y = 9;                                   // 5 units away
    g_frame = 175;                                                      // 75 frames = 7.5 s at 10 fps
    if (!Run(village, "IC")) return 1;
    g_frame = 176;
    if (!Run(village, "")) return 1;
    g_heroPos.X = 20;                                                   // more than 10 units away
    if (!Run(village, "IC")) return 1;
    g_heroPtr = 0;
    if (!Run(village, "")) return 1;
    g_criminal = false;
    if (!Run(village, "I")) return 1;
    std::printf("0065c08a_TEST PASS\n");
    return 0;
}