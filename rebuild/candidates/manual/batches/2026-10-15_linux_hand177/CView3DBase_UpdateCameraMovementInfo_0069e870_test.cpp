#include <cstdio>
// Behaviour model of CView3DBase::UpdateCameraMovementInfo: camera history shift, update counting, movement vector.
struct V { float X, Y, Z; };
struct Cam { V WorldPosition; float rest[11]; };
struct Info { long NoUpdates; bool Valid; Cam Current; Cam Last; V Move; };
static void Update(Info& i, const Cam& c)
{
    i.Last = i.Current;
    i.Current = c;
    if (i.NoUpdates > 2) {
        i.Move.X = i.Last.WorldPosition.X - i.Current.WorldPosition.X;
        i.Move.Y = i.Last.WorldPosition.Y - i.Current.WorldPosition.Y;
        i.Move.Z = i.Last.WorldPosition.Z - i.Current.WorldPosition.Z;
        i.Valid = true;
    }
    else
        ++i.NoUpdates;
}
int main()
{
    Info i; i.NoUpdates = 0; i.Valid = false; i.Current.WorldPosition.X = i.Current.WorldPosition.Y = i.Current.WorldPosition.Z = 0;
    for (int k = 1; k <= 4; ++k) {
        Cam c; c.WorldPosition.X = (float)k; c.WorldPosition.Y = 2.0f * k; c.WorldPosition.Z = 0.0f;
        Update(i, c);
    }
    if (i.NoUpdates != 3 || !i.Valid || i.Move.X != -1.0f || i.Move.Y != -2.0f || i.Last.WorldPosition.X != 3.0f) {
        std::printf("0069e870_TEST FAIL\n"); return 1;
    }
    std::printf("0069e870_TEST PASS\n");
    return 0;
}
