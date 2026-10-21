// Behaviour test for CAIStateGroup_Guildmaster::OnActivate (0x00977C40): model copy with scripted stubs.
#include <cstdio>
#include <cstring>
#include <vector>

struct C3DVector { float X, Y, Z; };
struct CTCCreatureModeManager {
    int modes[4]; int n;
    bool HasMode(unsigned long m) { for (int i = 0; i < n; ++i) if (modes[i] == (int)m) return true; return false; }
    bool AddMode(unsigned long m) { modes[n++] = (int)m; return true; }
};
struct CThing {
    C3DVector pos; const char* track; CTCCreatureModeManager* modes;
    const C3DVector& GetPosition() { return pos; }
    void GetInterface(CTCCreatureModeManager** o, long t) { if (t == 0x31) *o = modes; }
};
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; C2DBoxI(long a, long b, long c, long d) : MinX(a), MinY(b), MaxX(c), MaxY(d) { } };
struct CCharString { const char* s; CCharString(const char* t, long = -1) : s(t) { } };
struct CThingFilter_HasTrackName { CCharString Name; CThingFilter_HasTrackName(const CCharString& n) : Name(n) { } };
static CThing* g_world[8]; static int g_nworld;
static C2DBoxI g_lastBox(0, 0, 0, 0);
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, std::vector<CThing*>& out) const
    {
        g_lastBox = area;
        for (int i = 0; i < g_nworld; ++i) {
            CThing* t = g_world[i];
            if ((long)t->pos.X >= area.MinX && (long)t->pos.X <= area.MaxX && (long)t->pos.Y >= area.MinY && (long)t->pos.Y <= area.MaxY
                && t->track && !std::strcmp(t->track, filter.Name.s))
                out.push_back(t);
        }
    }
};
struct CIntelligentPointer { CThing* item; CIntelligentPointer(CThing* p) : item(p) { } };
struct CAIBrain { CThing* PCreature; };
static int g_bodyguards;

struct CAIStateGroup_Guildmaster {
    CAIBrain* PBrain; CThingSearchTools tools;
    std::vector<CIntelligentPointer> TeleportMarkers;
    long LastWillFrame, LastTeleportFrame;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools() { return &tools; }
    void FindAndStoreBodyguards() { ++g_bodyguards; }
    void OnActivate();
};

void CAIStateGroup_Guildmaster::OnActivate()
{
    FindAndStoreBodyguards();
    std::vector<CThing*> markers;
    {
        CCharString name("GuildmasterTeleportMarker");
        CThingFilter_HasTrackName filter(name);
        const C3DVector& pos = GetCreature()->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - 55, y - 55, x + 55, y + 55), filter, markers);
    }
    unsigned int count = (unsigned int)markers.size();
    for (unsigned int i = 0; i < count; ++i)
        TeleportMarkers.push_back(CIntelligentPointer(markers[i]));
    LastWillFrame = 0;
    LastTeleportFrame = 0;
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    if (!modes->HasMode(0x3b))
        modes->AddMode(0x3b);
}

int main()
{
    int ok = 1;
    CTCCreatureModeManager mm = { { 0 }, 0 };
    CThing gm = { { 100.7f, -20.2f, 0 }, 0, &mm };
    CThing m1 = { { 120, 10, 0 }, "GuildmasterTeleportMarker", 0 };
    CThing m2 = { { 300, 10, 0 }, "GuildmasterTeleportMarker", 0 };
    CThing m3 = { { 90, -60, 0 }, "GuildmasterTeleportMarker", 0 };
    CThing other = { { 100, -20, 0 }, "Other", 0 };
    g_world[0] = &m1; g_world[1] = &m2; g_world[2] = &other; g_world[3] = &m3; g_nworld = 4;
    CAIBrain b = { &gm };
    CAIStateGroup_Guildmaster s; s.PBrain = &b; s.LastWillFrame = 5; s.LastTeleportFrame = 6;
    s.OnActivate();
    if (g_bodyguards != 1 || g_lastBox.MinX != 45 || g_lastBox.MinY != -75 || g_lastBox.MaxX != 155 || g_lastBox.MaxY != 35) ok = 0;
    if (s.TeleportMarkers.size() != 2 || s.TeleportMarkers[0].item != &m1 || s.TeleportMarkers[1].item != &m3) ok = 0;
    if (s.LastWillFrame || s.LastTeleportFrame || mm.n != 1 || mm.modes[0] != 0x3b) ok = 0;
    // second activation: markers appended again, mode not added twice
    s.OnActivate();
    if (s.TeleportMarkers.size() != 4 || mm.n != 1) ok = 0;
    if (!ok) { std::printf("00977c40_TEST FAIL\n"); return 1; }
    std::printf("00977c40_TEST PASS\n");
    return 0;
}