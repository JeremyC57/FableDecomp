#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
class C3DBoundingBox { public:
    C3DVector MinCoord, MaxCoord;
    void InitialiseFromSphere(const C3DVector& centre, const C3DVector& extent)
    {
        MinCoord = centre - extent;
        MaxCoord = centre + extent;
    }
};
int main()
{
    C3DBoundingBox b;
    b.InitialiseFromSphere(C3DVector(1.0f, 2.0f, 3.0f), C3DVector(0.5f, 1.0f, 2.0f));
    if (b.MinCoord.X != 0.5f || b.MinCoord.Y != 1.0f || b.MinCoord.Z != 1.0f ||
        b.MaxCoord.X != 1.5f || b.MaxCoord.Y != 3.0f || b.MaxCoord.Z != 5.0f) {
        std::printf("00a31530_TEST FAIL\n"); return 1;
    }
    std::printf("00a31530_TEST PASS\n");
    return 0;
}