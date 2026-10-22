#include <cstdio>
#include <cstring>
// Model of the occlusion manager constructor: slots marked free, entry buffers zeroed, frame pointers aimed at the buffers.
struct Slot { long Handle; long RetrievalID; Slot() : Handle(-1) { } };
struct Entry { unsigned long Handle, Result; Entry() : Handle(0), Result(0) { } };
static int g_initCalls = 0;
struct CMgr {
    Slot Slots[2048];
    unsigned long NextSlot, CurrentFrame;
    Entry* Entries; Entry* PrevEntries;
    unsigned long EntriesSize, PrevEntriesSize;
    Entry Buffer1[64]; Entry Buffer2[64];
    const char* Names[3];
    CMgr()
    {
        std::memset(Slots, 0xff, sizeof(Slots));
        NextSlot = 0; CurrentFrame = 0;
        Entries = Buffer1; PrevEntries = Buffer2;
        EntriesSize = 0; PrevEntriesSize = 0;
        Names[0] = "VSHADER_STATIC_UNLIT";
        ++g_initCalls;
        Names[1] = "VSHADER_OCCLUSION_BOX";
        Names[2] = "PSHADER_DIFFUSE_ONLY";
    }
};
static CMgr g_mgr;
int main()
{
    int bad = 0;
    for (int i = 0; i < 2048; ++i) if (g_mgr.Slots[i].Handle != -1 || g_mgr.Slots[i].RetrievalID != -1) bad = 1;
    for (int i = 0; i < 64; ++i) if (g_mgr.Buffer1[i].Handle || g_mgr.Buffer2[i].Result) bad = 1;
    if (g_mgr.Entries != g_mgr.Buffer1 || g_mgr.PrevEntries != g_mgr.Buffer2) bad = 1;
    if (g_mgr.NextSlot || g_mgr.CurrentFrame || g_mgr.EntriesSize || g_mgr.PrevEntriesSize) bad = 1;
    if (g_initCalls != 1 || std::strcmp(g_mgr.Names[1], "VSHADER_OCCLUSION_BOX") != 0) bad = 1;
    if (bad) { std::printf("00b8d780_TEST FAIL\n"); return 1; }
    std::printf("00b8d780_TEST PASS\n");
    return 0;
}