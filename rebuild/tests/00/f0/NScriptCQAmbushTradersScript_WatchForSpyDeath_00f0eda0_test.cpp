// Behaviour test for 0x00F0EDA0 NScript::CQ_AmbushTradersScript::WatchForSpyDeath: each frame asks the spy whether it was
// killed (by anyone: an empty killer name); on a kill both SpyDead flags are set; the loop ends when the thread terminates.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CGameScriptThing { public:
    int KilledOnFrame; int* Frame; const char* LastAsked;
    bool MsgIsKilledBy(const CCharString& k) { LastAsked = k.Text; return *Frame >= KilledOnFrame; }
};
class CScriptThing { public:
    CGameScriptThing* Data;
    bool MsgIsKilledBy(const CCharString& k) const { if (!Data) return false; return Data->MsgIsKilledBy(k); }
};
static int g_frame, g_endFrame, g_termCalls;
class CGame { public: void NewScriptFrame() { ++g_frame; } };
struct CMaster { bool AmbushTradersSpyDead; };
struct CQ_AmbushTradersScript {
    CGame* Game; CMaster* MasterScriptData; bool SpyDead;
    bool IsActiveThreadTerminating() { ++g_termCalls; return g_frame >= g_endFrame; }
    void WatchForSpyDeath(CScriptThing spy)
    {
        for (;;) {
            if (IsActiveThreadTerminating())
                return;
            if (spy.MsgIsKilledBy("")) {
                if (IsActiveThreadTerminating())
                    return;
                SpyDead = true;
                MasterScriptData->AmbushTradersSpyDead = true;
            }
            Game->NewScriptFrame();
        }
    }
};

int main()
{
    CGame g; CMaster m; CQ_AmbushTradersScript q;
    CGameScriptThing imp; imp.Frame = &g_frame; imp.LastAsked = 0;
    CScriptThing spy; spy.Data = &imp;
    q.Game = &g; q.MasterScriptData = &m;
    // killed on frame 3, thread ends at frame 5
    g_frame = 0; g_endFrame = 5; imp.KilledOnFrame = 3; q.SpyDead = false; m.AmbushTradersSpyDead = false;
    q.WatchForSpyDeath(spy);
    int ok = q.SpyDead && m.AmbushTradersSpyDead && g_frame == 5 && imp.LastAsked && !std::strcmp(imp.LastAsked, "");
    // never killed
    g_frame = 0; g_endFrame = 4; imp.KilledOnFrame = 100; q.SpyDead = false; m.AmbushTradersSpyDead = false;
    q.WatchForSpyDeath(spy);
    ok = ok && !q.SpyDead && !m.AmbushTradersSpyDead && g_frame == 4;
    // null spy handle: never killed
    spy.Data = 0; g_frame = 0; g_endFrame = 2;
    q.WatchForSpyDeath(spy);
    ok = ok && !q.SpyDead && g_frame == 2;
    if (!ok) { std::printf("00f0eda0_TEST FAIL\n"); return 1; }
    std::printf("00f0eda0_TEST PASS\n");
    return 0;
}