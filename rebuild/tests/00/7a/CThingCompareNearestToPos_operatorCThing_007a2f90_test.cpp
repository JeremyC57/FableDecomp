// Behaviour test for CThingCompare_NearestToPos::operator()<CThing> (0x007A2F90): links against the source object's
// instantiation; CThing::GetPosition is defined here over a position at the start of the test thing.
#include <cstdio>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CThing {
public:
    const C3DVector& GetPosition() const;
};
struct STestThing { C3DVector Pos; STestThing(float x, float y, float z) : Pos(x, y, z) { } };
const C3DVector& CThing::GetPosition() const { return reinterpret_cast<const STestThing*>(this)->Pos; }
class CThingCompare_NearestToPos {
public:
    C3DVector Pos;
    template<class T> bool operator()(T* a, T* b) const;
};
static CThing* T(STestThing& t) { return reinterpret_cast<CThing*>(&t); }
int main()
{
    CThingCompare_NearestToPos c = { C3DVector(1.0f, 2.0f, 3.0f) };
    STestThing near_(1.0f, 2.0f, 5.0f), far_(4.0f, 2.0f, 3.0f), same(1.0f, 4.0f, 3.0f);   // distances^2: 4, 9, 4
    CThing* none = 0;
    int bad = 0;
    if (c(none, T(near_)) || c(none, none)) bad = 1;                    // null a never wins
    if (!c(T(far_), none)) bad = 2;                                     // null b always loses
    if (!c(T(near_), T(far_)) || c(T(far_), T(near_))) bad = 3;
    if (c(T(near_), T(same)) || c(T(same), T(near_))) bad = 4;          // equal distance: neither is less
    if (bad) { std::printf("007a2f90_TEST FAIL %d\n", bad); return 1; }
    std::printf("007a2f90_TEST PASS\n");
    return 0;
}
