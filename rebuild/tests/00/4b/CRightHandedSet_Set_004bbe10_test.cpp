#include <cstdio>

struct C3DVector { float X, Y, Z; };

struct CRightHandedSet {
    C3DVector Up; C3DVector Forward;
    void Set(const C3DVector& up, const C3DVector& forward) { Up = up; Forward = forward; }
};

int main()
{
    C3DVector up = { 0.0f, 1.0f, 0.0f }, fwd = { 1.5f, -0.5f, 4.0f };
    CRightHandedSet s; s.Up.X = s.Up.Y = s.Up.Z = 9.0f; s.Forward = s.Up;
    s.Set(up, fwd);
    if (s.Up.X != 0.0f || s.Up.Y != 1.0f || s.Up.Z != 0.0f || s.Forward.X != 1.5f || s.Forward.Y != -0.5f || s.Forward.Z != 4.0f)
    { std::printf("004bbe10_TEST FAIL\n"); return 1; }
    std::printf("004bbe10_TEST PASS\n");
    return 0;
}