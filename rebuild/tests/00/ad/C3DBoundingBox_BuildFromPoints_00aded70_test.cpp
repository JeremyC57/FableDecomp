// Behaviour test for 0x00ADED70 C3DBoundingBox::BuildFromPoints(const C3DVector*, float): zero box for a 0.0 count,
// otherwise the per-component min/max over the first ceil(count) points.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public:
    C3DVector MinCoord, MaxCoord;
    void BuildFromPoints(const C3DVector* points, float num_points)
    {
        if (num_points == 0.0f) {
            MinCoord.X = 0.0f; MinCoord.Y = 0.0f; MinCoord.Z = 0.0f;
            MaxCoord.X = 0.0f; MaxCoord.Y = 0.0f; MaxCoord.Z = 0.0f;
            return;
        }
        MinCoord = points[0];
        MaxCoord = points[0];
        for (unsigned int i = 0; i < num_points; ++i) {
            MinCoord.X = MinCoord.X < points[i].X ? MinCoord.X : points[i].X;
            MinCoord.Y = MinCoord.Y < points[i].Y ? MinCoord.Y : points[i].Y;
            MinCoord.Z = MinCoord.Z < points[i].Z ? MinCoord.Z : points[i].Z;
            MaxCoord.X = MaxCoord.X > points[i].X ? MaxCoord.X : points[i].X;
            MaxCoord.Y = MaxCoord.Y > points[i].Y ? MaxCoord.Y : points[i].Y;
            MaxCoord.Z = MaxCoord.Z > points[i].Z ? MaxCoord.Z : points[i].Z;
        }
    }
};

int main()
{
    int bad = 0;
    C3DVector pts[4] = { { 1, 2, 3 }, { -4, 5, 0 }, { 2, -6, 9 }, { 100, 100, -100 } };
    C3DBoundingBox b;
    b.BuildFromPoints(pts, 3.0f);
    if (b.MinCoord.X != -4 || b.MinCoord.Y != -6 || b.MinCoord.Z != 0 || b.MaxCoord.X != 2 || b.MaxCoord.Y != 5 || b.MaxCoord.Z != 9) bad = 1;
    b.BuildFromPoints(pts, 1.0f);                        // a single point
    if (b.MinCoord.X != 1 || b.MaxCoord.Z != 3 || b.MinCoord.Y != 2) bad = 1;
    b.BuildFromPoints(pts, 3.5f);                        // a fractional count visits index 3 too
    if (b.MaxCoord.X != 100 || b.MinCoord.Z != -100) bad = 1;
    b.BuildFromPoints(pts, 0.0f);
    if (b.MinCoord.X != 0 || b.MinCoord.Y != 0 || b.MinCoord.Z != 0 || b.MaxCoord.X != 0 || b.MaxCoord.Y != 0 || b.MaxCoord.Z != 0) bad = 1;
    if (bad) { std::printf("00aded70_TEST FAIL\n"); return 1; }
    std::printf("00aded70_TEST PASS\n");
    return 0;
}