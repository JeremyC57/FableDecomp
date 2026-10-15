// Behaviour test for CTCHaste::CTCHaste (0x004E0C61): base gets the thing, Multiplier 1.0f, ToTerminate false, PauseFrames -1, map built.
#include <cstdio>
struct CThing { int m_id; };
struct CTCBase {
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    virtual ~CTCBase() {}
};
static int g_mapsBuilt = 0;
struct CHasteMultiplierMap { unsigned long _Mysize; CHasteMultiplierMap() : _Mysize(0) { ++g_mapsBuilt; } };
struct CTCHaste : CTCBase {
    float Multiplier; bool ToTerminate; long PauseFrames; CHasteMultiplierMap Multipliers;
    CTCHaste(CThing* thing) : CTCBase(thing), Multiplier(1.0f), ToTerminate(false), PauseFrames(-1) {}
};
int main()
{
    CThing t; t.m_id = 1;
    CTCHaste h(&t);
    if (h.ParentThing != &t || h.Multiplier != 1.0f || h.ToTerminate || h.PauseFrames != -1 || g_mapsBuilt != 1) { std::printf("004e0c61_TEST FAIL\n"); return 1; }
    std::printf("004e0c61_TEST PASS\n");
    return 0;
}