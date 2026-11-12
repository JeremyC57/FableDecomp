// Behaviour test for CGameCameraManager::PreparePrimitivesForRendering (0x006992A0): a copy of the control flow over a small model.
#include <cstdio>
#include <list>
#include <utility>
struct Vec { float X, Y, Z; };
struct Colour { unsigned char B, G, R, A; };
struct Line { long Type; Vec Start, End; Colour C; float Width; bool UseZ; };
struct Thing { Vec Pos; bool HasCamPoints; bool HasShape; };
struct Log { int modeDraws, pyramids, shapeDraws, lines; float lastSize; Line last; };
struct Model {
    long DrawDebugGraphics; Vec ModePos; std::list<Thing*> World; std::list<std::pair<Vec, Colour> > DebugPoints;
    void Prepare(Log& log) const
    {
        if (DrawDebugGraphics == 0) return;
        if (DrawDebugGraphics == 1 || DrawDebugGraphics == 3) ++log.modeDraws;
        if (DrawDebugGraphics == 2 || DrawDebugGraphics == 3) {
            std::list<Thing*> things;
            for (std::list<Thing*>::const_iterator w = World.begin(); w != World.end(); ++w)
                if ((*w)->HasCamPoints) things.push_back(*w);
            for (std::list<Thing*>::iterator it = things.begin(); it != things.end(); ++it) {
                ++log.pyramids; log.lastSize = 0.25f;
                if ((*it)->HasShape) ++log.shapeDraws;
            }
            Vec pos = ModePos;
            for (std::list<std::pair<Vec, Colour> >::const_iterator p = DebugPoints.begin(); p != DebugPoints.end(); ++p) {
                std::pair<Vec, Colour> point = *p;
                Line l = { 6, pos, point.first, point.second, 0.05f, true };
                log.last = l; ++log.lines;
            }
        }
    }
};
int main()
{
    Thing a = { {1, 2, 3}, true, true }, b = { {4, 5, 6}, true, false }, c = { {7, 8, 9}, false, true };
    Model m; m.ModePos.X = 10; m.ModePos.Y = 20; m.ModePos.Z = 30;
    m.World.push_back(&a); m.World.push_back(&b); m.World.push_back(&c);
    Vec e = { -1, -2, -3 }; Colour col = { 1, 2, 3, 4 };
    m.DebugPoints.push_back(std::make_pair(e, col)); m.DebugPoints.push_back(std::make_pair(e, col));
    int expect[4][4] = { {0,0,0,0}, {1,0,0,0}, {0,2,1,2}, {1,2,1,2} };
    for (long mode = 0; mode < 4; ++mode) {
        Log log = { 0, 0, 0, 0, 0 };
        m.DrawDebugGraphics = mode; m.Prepare(log);
        if (log.modeDraws != expect[mode][0] || log.pyramids != expect[mode][1] || log.shapeDraws != expect[mode][2] || log.lines != expect[mode][3]) {
            std::printf("006992a0_TEST FAIL\n"); return 1;
        }
        if (mode >= 2 && (log.last.Type != 6 || log.last.Start.Z != 30 || log.last.End.Y != -2 || log.last.C.A != 4 || log.last.Width != 0.05f || !log.last.UseZ || log.lastSize != 0.25f)) {
            std::printf("006992a0_TEST FAIL\n"); return 1;
        }
    }
    std::printf("006992a0_TEST PASS\n");
    return 0;
}