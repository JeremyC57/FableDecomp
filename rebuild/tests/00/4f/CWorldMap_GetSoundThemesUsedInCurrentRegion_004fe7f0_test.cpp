// Behaviour test for 0x004FE7F0 CWorldMap::GetSoundThemesUsedInCurrentRegion(set<unsigned long>&) const: every
// game-map cell (rows indexed bottom-up) of each map of the current region contributes its SoundTheme to a sorted,
// de-duplicated local set; each theme whose SOUND_THEME class index resolves (> 0) to a definition adds that def's
// switch ID to the output set, and each looked-up definition reference is released again.
#include <cstdio>

class CGameMap { public:
    int SizeX, SizeY;
    signed char Cells[16];                     // SoundTheme per cell, row-major from the bottom row
    signed char PeekSoundTheme(int x, int y) const { return Cells[(SizeY - y - 1) * SizeX + x]; }
};
static CGameMap g_maps[2] = {
    { 2, 2, { 3, 1, 3, 2 } },
    { 3, 1, { 5, 1, 7 } },
};
class CRegion { public: int MapNumbers[2]; int MapCount; };
static CRegion g_regions[2] = { { { 0, 0 }, 0 }, { { 0, 1 }, 2 } };

// themes 1..7: global index = theme * 10 for even themes, 0 (unknown) for odd ones except 1 and 5 (index 15 / 55)
static int GetGlobalIndexFromClassIndex(int theme)
{
    if (theme == 1) return 15;
    if (theme == 5) return 55;
    return (theme % 2 == 0) ? theme * 10 : 0;
}
class CSoundThemeDef { public: int RefCount; int SwitchID; };
static CSoundThemeDef g_defs[80];
static int g_lookups = 0, g_released = 0;
static bool GetDef(int index, CSoundThemeDef*& def)
{
    ++g_lookups;
    if (index == 55) return false;                 // no definition behind this index
    def = &g_defs[index]; def->RefCount = 1; def->SwitchID = 1000 + index;
    return true;
}

static void Collect(int currentRegion, unsigned int* out, int& outCount)
{
    const CRegion& region = g_regions[currentRegion];
    bool used[128] = { false };
    for (int i = 0; i < region.MapCount; ++i) {
        const CGameMap& map = g_maps[region.MapNumbers[i]];
        for (int x = 0; x < map.SizeX; ++x)
            for (int y = 0; y < map.SizeY; ++y)
                used[map.PeekSoundTheme(x, y)] = true;
    }
    for (int theme = 0; theme < 128; ++theme) {    // ascending, as the set iterates
        if (!used[theme]) continue;
        CSoundThemeDef* def = 0;
        int index = GetGlobalIndexFromClassIndex(theme);
        if (index > 0) {
            if (GetDef(index, def)) {
                unsigned int id = (unsigned int)def->SwitchID;
                bool dup = false;
                for (int k = 0; k < outCount; ++k) if (out[k] == id) dup = true;
                if (!dup) out[outCount++] = id;
            }
        }
        if (def && --def->RefCount == 0) ++g_released;
    }
}

int main()
{
    int ok = 1;
    unsigned int out[16]; int count = 0;
    Collect(1, out, count);                        // themes {1,2,3,5,7}: 1 -> 1015, 2 -> 1020, 5 -> no def
    if (count != 2 || out[0] != 1015 || out[1] != 1020 || g_lookups != 3 || g_released != 2) ok = 0;
    count = 0;
    Collect(0, out, count);                        // no maps in region 0
    if (count != 0 || g_lookups != 3) ok = 0;
    if (!ok) { std::printf("004fe7f0_TEST FAIL\n"); return 1; }
    std::printf("004fe7f0_TEST PASS\n");
    return 0;
}