// Behaviour test for 0x00817010 swap<THitLocationIntersectionElement> (model copy).
#include <cstdio>
struct CHitLocation { int id; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct THitLocationIntersectionElement {
    long          key;
    CHitLocation* hit_location;
    C3DVector     intersection_pos;
    C3DVector     outward_normal;
};
template <class T> inline void swap_model(T& a, T& b) { T tmp = a; a = b; b = tmp; }
int main()
{
    CHitLocation h1, h2;
    THitLocationIntersectionElement a, b;
    a.key = 1; a.hit_location = &h1; a.intersection_pos = C3DVector(1, 2, 3); a.outward_normal = C3DVector(4, 5, 6);
    b.key = 2; b.hit_location = &h2; b.intersection_pos = C3DVector(7, 8, 9); b.outward_normal = C3DVector(10, 11, 12);
    swap_model(a, b);
    if (a.key != 2 || a.hit_location != &h2 || a.intersection_pos.Y != 8 || a.outward_normal.Z != 12 ||
        b.key != 1 || b.hit_location != &h1 || b.intersection_pos.X != 1 || b.outward_normal.Y != 5) {
        std::printf("00817010_TEST FAIL\n"); return 1;
    }
    std::printf("00817010_TEST PASS\n");
    return 0;
}