// Behaviour test for CWaypointSpeedCheck::Main (0x00DDC6C0): best squared distance and waypoint number tracking,
// with the reset-to-1000 mode for data string "0", until the thread terminates.
#include <cstdio>
#include <cstring>
#include <cstdlib>
class CCharString { public: const char* Text; bool operator==(const char* t) const { return std::strcmp(Text ? Text : "", t) == 0; } };
static long GFCharStringToInt(const CCharString& s) { return std::atol(s.Text); }
static const char* g_data; static float g_dist[8]; static int g_frame, g_termAt, g_termCalls;
class CScriptThing { public: CCharString GetDataString() const { CCharString c = { g_data }; return c; } };
static float GetSquaredDistanceBetweenThings(const CScriptThing&, const CScriptThing&) { return g_dist[g_frame]; }
class CGameScriptInterface { public: CScriptThing Hero; void NewScriptFrame() const { ++g_frame; } CScriptThing& GetHero() { return Hero; } };
struct CWaypointSpeedCheckData { float BestDistanceSquared; long BestWaypointNumber; };
struct CWaypointSpeedCheck {
    CGameScriptInterface* Game; CScriptThing Me; CWaypointSpeedCheckData* Data;
    bool IsActiveThreadTerminating() { return ++g_termCalls >= g_termAt; }
    void Main()
    {
        bool resetEveryFrame = Me.GetDataString() == "0";
        while (!IsActiveThreadTerminating()) {
            if (resetEveryFrame) {
                if (IsActiveThreadTerminating())
                    return;
                Data->BestDistanceSquared = 1000.0f;
            }
            float distance = GetSquaredDistanceBetweenThings(Me, Game->GetHero());
            if (distance < Data->BestDistanceSquared) {
                if (IsActiveThreadTerminating())
                    return;
                Data->BestDistanceSquared = distance;
                Data->BestWaypointNumber = GFCharStringToInt(Me.GetDataString());
            }
            Game->NewScriptFrame();
        }
    }
};
int main()
{
    float d[4] = { 50.0f, 20.0f, 30.0f, 10.0f };
    for (int i = 0; i < 4; ++i) g_dist[i] = d[i];
    CGameScriptInterface g; CWaypointSpeedCheckData data = { 40.0f, -1 }; CWaypointSpeedCheck w; w.Game = &g; w.Data = &data;
    g_data = "7"; g_frame = 0; g_termCalls = 0; g_termAt = 6;   // frames 0..2: 50 (no), 20 (best), 30 (no)
    w.Main();
    float b = 20.0f; bool ok = data.BestDistanceSquared == b && data.BestWaypointNumber == 7 && g_frame == 3;
    data.BestDistanceSquared = 5.0f; data.BestWaypointNumber = -1;
    g_data = "0"; g_frame = 0; g_termCalls = 0; g_termAt = 4;   // reset mode: first frame resets to 1000, 50 becomes best
    w.Main();
    float f = 50.0f; ok = ok && data.BestDistanceSquared == f && data.BestWaypointNumber == 0;
    if (!ok) { std::printf("00ddc6c0_TEST FAIL\n"); return 1; }
    std::printf("00ddc6c0_TEST PASS\n");
    return 0;
}