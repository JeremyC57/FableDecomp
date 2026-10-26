// Behaviour test for 006bc210: the model below is a copy of src/006bc210.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_assigns = 0, g_moves = 0;
using std::memmove;
class CCharString { public:
    const char* PStringData;
    CCharString& operator=(const char* text) { ++g_assigns; PStringData = text; return *this; }
};
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    if (last != first) ++g_moves;
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    long* erase(long* first, long* last)
    {
        long* i = (long*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
class C2DVector { public:
    void Set(float x, float y) { X = x; Y = y; }
    float X; float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CRegion { public:
    CLongVector Maps;
    CLongVector VisMaps;
    CCharString Name;
    CCharString DisplayName;
    CCharString RegionDefName;
    void*       PDef;
    CCharString MiniMapGraphic;
    float       MiniMapScale;
    C2DVector   MiniMapOffset;
    void*       MiniMapRegionExitTextOffsets[3];
    C2DVector   WorldMapOffset;
    C2DVector   NameGraphicOffset;
    bool        AppearOnWorldMap;
    bool        CreatureGenerationEnabled;
    bool        SoundThemesEnabled;
    void Reset();
};
void CRegion::Reset()
{
    Maps.clear();
    Name = "";
    DisplayName = "";
    RegionDefName = "";
    AppearOnWorldMap = true;
    CreatureGenerationEnabled = true;
    SoundThemesEnabled = true;
    MiniMapGraphic = "";
    MiniMapScale = 1.0f;
    MiniMapOffset.Set(0.0f, 0.0f);
    WorldMapOffset.Set(0.0f, 0.0f);
}
static bool Run(int maps)
{
    static long storage[4] = { 1, 2, 3, 4 };
    CRegion r; std::memset(&r, 0x5a, sizeof(r));
    r.Maps._M_start = storage; r.Maps._M_finish = storage + maps; r.Maps._M_end_of_storage = storage + 4;
    r.VisMaps._M_start = storage; r.VisMaps._M_finish = storage + 2; r.VisMaps._M_end_of_storage = storage + 4;
    r.Name.PStringData = "n"; r.DisplayName.PStringData = "d"; r.RegionDefName.PStringData = "r"; r.MiniMapGraphic.PStringData = "g";
    r.NameGraphicOffset.X = 7.0f; r.NameGraphicOffset.Y = 8.0f;
    g_assigns = g_moves = 0;
    r.Reset();
    if (r.Maps._M_finish != storage || r.Maps._M_start != storage) return false;         // cleared (erase to the start, no copy)
    if (g_moves != 0 || r.VisMaps._M_finish != storage + 2) return false;                // VisMaps untouched
    if (g_assigns != 4) return false;
    if (std::strcmp(r.Name.PStringData, "") || std::strcmp(r.DisplayName.PStringData, "") || std::strcmp(r.RegionDefName.PStringData, "") || std::strcmp(r.MiniMapGraphic.PStringData, "")) return false;
    if (!r.AppearOnWorldMap || !r.CreatureGenerationEnabled || !r.SoundThemesEnabled) return false;
    if (r.MiniMapScale != 1.0f || r.MiniMapOffset.X != 0.0f || r.MiniMapOffset.Y != 0.0f || r.WorldMapOffset.X != 0.0f || r.WorldMapOffset.Y != 0.0f) return false;
    if (r.NameGraphicOffset.X != 7.0f || r.NameGraphicOffset.Y != 8.0f) return false;   // not part of the reset
    return true;
}
int main()
{
    bool ok = Run(3) && Run(0);
    if (!ok) { std::printf("006bc210_TEST FAIL\n"); return 1; }
    std::printf("006bc210_TEST PASS\n");
    return 0;
}