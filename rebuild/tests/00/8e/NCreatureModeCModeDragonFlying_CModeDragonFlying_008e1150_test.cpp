// Behaviour test for CModeDragonFlying::CModeDragonFlying (0x008E1150): spline = position + points, destination = last point.
#include <cstdio>

struct V { float X, Y, Z; };
struct Info { long RefCount; };
struct Mode {
    V Control[8]; unsigned long ControlCount; float Tension; bool EndPointsConnected;
    V Destination, StartPos; float Speed; Info* PEndInfo; long NumAnimLoops, NumAnimLoopsToDo; bool UsePreviousPos;
    float SplinePercentageComplete;
    void Build(const V* points, unsigned long count, const V& position, float speed, Info* endAction)
    {
        Tension = 0.5f; EndPointsConnected = false; ControlCount = 0; PEndInfo = 0;
        Speed = speed;
        if (PEndInfo != endAction) { PEndInfo = endAction; if (PEndInfo) ++PEndInfo->RefCount; }
        StartPos = points[0];
        Destination = points[count - 1];
        ControlCount = count + 1;
        NumAnimLoopsToDo = count - 1;
        Control[0] = position;
        UsePreviousPos = true;
        for (unsigned long i = 0; i < count; ++i) Control[i + 1] = points[i];
        SplinePercentageComplete = 0;
        NumAnimLoops = 0;
        if (endAction) --endAction->RefCount;   // the by-value parameter is released by the callee
    }
};

int main()
{
    V pts[3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    V pos = { -1, -2, -3 };
    Info info; info.RefCount = 1;
    ++info.RefCount;                           // caller's copy for the by-value argument
    Mode m; m.Build(pts, 3, pos, 2.5f, &info);
    if (m.ControlCount != 4 || m.Control[0].X != -1 || m.Control[3].Z != 9 || m.Destination.Y != 8 || m.StartPos.X != 1
        || m.NumAnimLoopsToDo != 2 || !m.UsePreviousPos || m.Speed != 2.5f || info.RefCount != 2 || m.Tension != 0.5f) {
        std::printf("008e1150_TEST FAIL\n"); return 1;
    }
    std::printf("008e1150_TEST PASS\n");
    return 0;
}