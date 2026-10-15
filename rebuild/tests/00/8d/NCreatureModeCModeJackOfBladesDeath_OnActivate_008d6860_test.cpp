#include <cstdio>
#include <cstring>
#include <cstdlib>

struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
struct CThing { C3DVector pos; const C3DVector& GetPosition() { return pos; } };
struct CModeBase { virtual ~CModeBase() { } CThing* thing; };
struct CMode : CModeBase { C3DVector start; long counter; virtual void OnActivate() { start = thing->GetPosition(); counter = 0; } };
int main()
{
    CThing t; t.pos.X = 1.5f; t.pos.Y = -2.0f; t.pos.Z = 9.0f; CMode m; m.thing = &t; m.counter = 77;
    m.OnActivate(); if (m.start.X != 1.5f || m.start.Y != -2.0f || m.start.Z != 9.0f || m.counter != 0) { std::printf("008d6860_TEST FAIL\n"); return 1; }
    std::printf("008d6860_TEST PASS\n");
    return 0;
}