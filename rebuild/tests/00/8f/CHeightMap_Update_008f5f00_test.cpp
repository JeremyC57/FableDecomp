// Behaviour test for 0x008F5F00 (CHeightMap::Update): every cell of every queued area loses its Changed bit; others keep it; the queue empties.
#include <cstdio>
struct Cell { float Height; unsigned char Pal[3]; unsigned char Blend[2]; unsigned char Changed : 1; unsigned char Passable : 1; unsigned char rest : 6; };
struct Box { int MinX, MinY, MaxX, MaxY; };
struct HeightMap {
    Cell* first; Box areas[30]; int NoMapUpdateAreas; int SizeX, SizeY;
    void Update()
    {
        for (int i = 0; i < NoMapUpdateAreas; ++i) {
            Box& area = areas[i];
            for (int y = area.MinY; y < area.MaxY; ++y) {
                Cell* cell = &first[(SizeY - y) * (SizeX + 1) + area.MinX];
                for (int x = area.MinX; x < area.MaxX; ++x, ++cell)
                    cell->Changed = 0;
            }
        }
        NoMapUpdateAreas = 0;
    }
};
int main()
{
    static Cell cells[9 * 9];
    for (int i = 0; i < 81; ++i) { cells[i].Changed = 1; cells[i].Passable = 1; }
    HeightMap m; m.first = cells; m.SizeX = 8; m.SizeY = 8; m.NoMapUpdateAreas = 2;
    Box a = { 1, 1, 3, 3 }; Box b = { 5, 6, 6, 8 }; m.areas[0] = a; m.areas[1] = b;
    m.Update();
    int ok = m.NoMapUpdateAreas == 0;
    for (int y = 0; y <= 8; ++y)
        for (int x = 0; x <= 8; ++x) {
            bool inA = x >= 1 && x < 3 && y >= 1 && y < 3, inB = x >= 5 && x < 6 && y >= 6 && y < 8;
            const Cell& c = cells[(8 - y) * 9 + x];
            if (c.Changed != ((inA || inB) ? 0 : 1) || c.Passable != 1) ok = 0;
        }
    if (!ok) { std::printf("008f5f00_TEST FAIL\n"); return 1; }
    std::printf("008f5f00_TEST PASS\n");
    return 0;
}