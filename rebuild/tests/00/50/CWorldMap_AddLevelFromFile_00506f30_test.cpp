// Behaviour test for CWorldMap::AddLevelFromFile (0x00506F30): first free slot from index 1 reused, otherwise the array
// grows; the slot is filled, its box moved to the position, UID allocated and the map registered.
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; } };
struct C2DCoordI { long X, Y; };
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMapInfo { C2DBoxI Area; CCharString LevelName; unsigned long MapUID; bool Used, Placed; };
struct CArray {
    CMapInfo Store[8]; CMapInfo* _M_start; CMapInfo* _M_finish;
    unsigned int size() const { return _M_finish - _M_start; }
    CMapInfo& operator[](unsigned int i) { return *(_M_start + i); }
    void resize(unsigned int n) { CMapInfo blank = { { 0, 0, 0, 0 }, { 0 }, 0, false, false }; while (size() < n) *_M_finish++ = blank; }
};
static long g_added = -1, g_loaded = -1;
struct CEngineManager { static CEngineManager& Get() { static CEngineManager m; return m; } void AddMap(long m) { g_added = m; } };
struct CWorldMap {
    CArray Maps;
    unsigned long AllocateMapUID(long map) { return 1000 + map; }
    void LoadLoadableGlobalThingsForLevel(long map) { g_loaded = map; }
    C2DBoxI GetLevelDimensions(const CCharString&) { C2DBoxI b = { 0, 0, 5, 7 }; return b; }
    long AddLevelFromFile(const C2DCoordI& position, const CCharString& levelName)
    {
        CMapInfo* info = 0;
        unsigned long map;
        for (map = 1; map < Maps.size(); ++map) {
            if (!Maps[map].Used) { info = &Maps[map]; break; }
        }
        if (!info) { Maps.resize(Maps.size() + 1); map = Maps.size() - 1; info = &Maps[map]; }
        info->Used = true;
        info->LevelName = levelName;
        info->Placed = false;
        info->Area = GetLevelDimensions(levelName);
        info->Area.BRX += position.X - info->Area.TLX;
        info->Area.BRY += position.Y - info->Area.TLY;
        info->Area.TLX = position.X;
        info->Area.TLY = position.Y;
        info->MapUID = AllocateMapUID(map);
        CEngineManager::Get().AddMap(map);
        LoadLoadableGlobalThingsForLevel(map);
        return map;
    }
};
int main()
{
    CWorldMap w; std::memset(&w.Maps.Store, 0, sizeof w.Maps.Store);
    w.Maps._M_start = w.Maps.Store; w.Maps._M_finish = w.Maps.Store + 3;
    w.Maps.Store[0].Used = false; w.Maps.Store[1].Used = true; w.Maps.Store[2].Used = false; w.Maps.Store[2].Placed = true;
    CCharString n = { "Lev" }; C2DCoordI p = { 10, 20 };
    long a = w.AddLevelFromFile(p, n);
    CMapInfo& m = w.Maps.Store[2];
    bool ok = a == 2 && m.Used && !m.Placed && m.LevelName.Text == n.Text && m.Area.TLX == 10 && m.Area.TLY == 20 && m.Area.BRX == 15
        && m.Area.BRY == 27 && m.MapUID == 1002 && g_added == 2 && g_loaded == 2 && w.Maps.size() == 3;
    long b = w.AddLevelFromFile(p, n);
    ok = ok && b == 3 && w.Maps.size() == 4 && w.Maps.Store[3].Used && g_added == 3;
    if (!ok) { std::printf("00506f30_TEST FAIL\n"); return 1; }
    std::printf("00506f30_TEST PASS\n");
    return 0;
}