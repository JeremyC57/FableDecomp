#include <cstdio>
#include <cstring>
#include <new>

class CGameMapCell { public:
    unsigned char AtmosThemes[3]; unsigned char AtmosBlends[2]; signed char SoundTheme; unsigned char Music;
    void Clear() { std::memset(AtmosThemes, 0, sizeof(AtmosThemes)); std::memset(AtmosBlends, 0, sizeof(AtmosBlends));
        AtmosThemes[0] = 0; AtmosBlends[0] = 0xff; SoundTheme = -1; Music = 0; }
};
class CGameMap { public: void* vt; CGameMapCell* b; CGameMapCell* e; CGameMapCell* cap;
    void Clear() { for (CGameMapCell* cell = b; cell != e; ++cell) cell->Clear(); } };
int main()
{
    CGameMapCell cells[3]; std::memset(cells, 0x5a, sizeof cells);
    CGameMap map; map.b = cells; map.e = cells + 3; map.Clear();
    for (int i = 0; i < 3; ++i) {
        const CGameMapCell& c = cells[i];
        if (c.AtmosThemes[0] || c.AtmosThemes[1] || c.AtmosThemes[2] || c.AtmosBlends[0] != 0xff || c.AtmosBlends[1] || c.SoundTheme != -1 || c.Music) { std::printf("006b7510_TEST FAIL\n"); return 1; }
    }
    map.e = map.b; cells[0].Music = 9; map.Clear(); if (cells[0].Music != 9) { std::printf("006b7510_TEST FAIL\n"); return 1; }      // empty range: untouched
    std::printf("006b7510_TEST PASS\n");
    return 0;
}