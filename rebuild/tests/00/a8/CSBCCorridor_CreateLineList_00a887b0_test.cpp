// Behaviour test for CSBCCorridor::CreateLineList (0x00A887B0), g++ model of the retail logic.
#include <cstdio>
#include <list>
#include <vector>
struct V2 { float X, Y; };
struct V3 { float X, Y, Z; };
struct Wall { V2 Point1, Point2, ToInternal; };
static void CreateLineList(const std::list<Wall>& walls, std::vector<V3>& lines)
{
    for (std::list<Wall>::const_iterator it = walls.begin(); it != walls.end(); ++it) {
        V3 start = { it->Point1.X, it->Point1.Y, 0.0f };
        V3 end = { it->Point2.X, it->Point2.Y, 0.0f };
        lines.push_back(start);
        lines.push_back(end);
    }
}
int main()
{
    std::list<Wall> walls;
    Wall a = { { 1, 2 }, { 3, 4 }, { 9, 9 } }, b = { { -1, 5 }, { 6, -7 }, { 9, 9 } };
    walls.push_back(a); walls.push_back(b);
    std::vector<V3> lines(1);
    lines[0].X = 42; lines[0].Y = 0; lines[0].Z = 1;
    CreateLineList(walls, lines);
    int ok = lines.size() == 5 && lines[0].X == 42 && lines[1].X == 1 && lines[1].Y == 2 && lines[2].X == 3 && lines[2].Y == 4
          && lines[3].X == -1 && lines[4].Y == -7 && lines[1].Z == 0 && lines[4].Z == 0;
    std::vector<V3> none; CreateLineList(std::list<Wall>(), none);
    ok = ok && none.empty();
    std::printf(ok ? "00a887b0_TEST PASS\n" : "00a887b0_TEST FAIL\n");
    return ok ? 0 : 1;
}