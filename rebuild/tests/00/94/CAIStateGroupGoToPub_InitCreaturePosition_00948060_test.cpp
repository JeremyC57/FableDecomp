// Behaviour test for CAIStateGroup_GoToPub::InitCreaturePosition (0x00948060): model copy with scripted stubs.
#include <cstdio>
#include <cstring>

struct CThing { unsigned char m_flags91; void* owner; };
struct CTCAIScratchpad { const char* TableName; const char* SeatName; };
static CThing* g_marker;
static long g_frame, g_placed;
static bool g_runnable, g_placeOk;
static CThing* g_placedNear;
struct CAIStateGroup_GoToPub {
    CTCAIScratchpad* scratchpad;
    CThing* PPub;
    CThing* PTable;
    long NextFramePub;
    bool UnderScriptControl;
    CThing* FindMarkerByName(const char*) { return g_marker; }
    bool IsRunnable() { return g_runnable; }
    bool AutoPlaceNearBuilding(CThing* b, float d) { ++g_placed; g_placedNear = b; return g_placeOk && d == 3.0f; }
    bool InitCreaturePosition()
    {
        UnderScriptControl = std::strcmp(scratchpad->SeatName, "") != 0 && std::strcmp(scratchpad->TableName, "") != 0;
        if (UnderScriptControl) {
            PTable = FindMarkerByName(scratchpad->TableName);
            CThing* owner = PTable->owner ? (CThing*)PTable->owner : 0;
            if (PTable->owner) {
                PPub = owner;
                if (PPub != 0 && !(PPub->m_flags91 & 1) && AutoPlaceNearBuilding(PPub, 3.0f))
                    return true;
            }
        } else if (NextFramePub < g_frame && IsRunnable() && AutoPlaceNearBuilding(PPub, 3.0f))
            return true;
        return false;
    }
};

static int fail() { std::printf("00948060_TEST FAIL\n"); return 1; }

int main()
{
    CThing pub = { 0, 0 };
    CThing table = { 0, &pub };
    CTCAIScratchpad sp = { "TABLE_1", "SEAT_2" };
    CAIStateGroup_GoToPub g;
    g.scratchpad = &sp; g.PPub = 0; g.PTable = 0; g.NextFramePub = 100;
    g_marker = &table; g_placeOk = true; g_frame = 50; g_runnable = true;
    if (!g.InitCreaturePosition() || !g.UnderScriptControl || g.PTable != &table || g.PPub != &pub || g_placedNear != &pub) return fail();
    pub.m_flags91 = 1;
    if (g.InitCreaturePosition()) return fail();
    // unscripted: needs the frame to pass NextFramePub and IsRunnable
    sp.SeatName = ""; pub.m_flags91 = 0; g_placed = 0;
    if (g.InitCreaturePosition() || g.UnderScriptControl || g_placed != 0) return fail();
    g_frame = 101;
    if (!g.InitCreaturePosition() || g_placed != 1) return fail();
    g_runnable = false;
    if (g.InitCreaturePosition()) return fail();
    std::printf("00948060_TEST PASS\n");
    return 0;
}