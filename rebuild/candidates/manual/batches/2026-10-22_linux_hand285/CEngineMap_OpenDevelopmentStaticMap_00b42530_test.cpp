#include <cstdio>
// Model of OpenDevelopmentStaticMap's control flow: early outs, dimension check, map-1 async open, detail cache, map ID record.
static int g_closed, g_entryCreated, g_landscapeOpened, g_detailOpened;
static unsigned long g_positions[8]; static int g_posCount;
struct Info { unsigned long LandscapeOffset, DetailOffset, EndOffset; long W, H, D, C; long Ext[6]; };
struct Map {
    long ID; long Ext[6]; unsigned short W, H, D, C; bool HasDetail; unsigned long FoundIndex; Info Header;
    void Open(long mapID)
    {
        if (ID == mapID) return;
        ++g_closed;
        unsigned long index = FoundIndex;
        if (!index) return;
        Info info = Header;
        if (info.W == W && info.H == H && info.D == D && info.C == C) {
            for (int i = 0; i < 6; ++i) Ext[i] = info.Ext[i];
            if (mapID == 1) {
                ++g_entryCreated;
                g_positions[g_posCount++] = info.LandscapeOffset; ++g_landscapeOpened;
                if (HasDetail) { g_positions[g_posCount++] = info.DetailOffset; ++g_detailOpened; }
            }
            g_positions[g_posCount++] = info.EndOffset;
            ID = mapID;
        }
    }
};
int main()
{
    int bad = 0;
    Map m; m.ID = 0; m.W = 4; m.H = 5; m.D = 6; m.C = 7; m.HasDetail = true; m.FoundIndex = 3;
    for (int i = 0; i < 6; ++i) { m.Ext[i] = 0; m.Header.Ext[i] = i + 10; }
    m.Header.W = 4; m.Header.H = 5; m.Header.D = 6; m.Header.C = 7;
    m.Header.LandscapeOffset = 100; m.Header.DetailOffset = 200; m.Header.EndOffset = 300;
    m.Open(0); if (g_closed != 0) bad = 1;
    m.Open(1);
    if (m.ID != 1 || g_entryCreated != 1 || g_detailOpened != 1 || g_posCount != 3 || g_positions[1] != 200 || m.Ext[5] != 15) bad = 1;
    m.Header.C = 8; m.Open(2); if (m.ID != 1 || g_closed != 2) bad = 1;
    m.Header.C = 7; m.Open(2); if (m.ID != 2 || g_entryCreated != 1 || g_positions[g_posCount - 1] != 300) bad = 1;
    m.FoundIndex = 0; m.Open(5); if (m.ID != 2) bad = 1;
    if (bad) { std::printf("00b42530_TEST FAIL\n"); return 1; }
    std::printf("00b42530_TEST PASS\n");
    return 0;
}
