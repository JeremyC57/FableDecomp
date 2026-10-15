// Behaviour test for 0075c1e0 (CTCCameraPointShapeActivationBase::GetTrackPosition): returns a copy of the parent thing's position.
#include <cstdio>

struct C3DVector {
    long X;
    long Y;
    long Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CThing {
    C3DVector m_position;
    const C3DVector& GetPosition() const { return m_position; }
};

struct CTCBase { CThing* ParentThing; };

struct CTCCameraPointShapeActivationBase : CTCBase {
    C3DVector GetTrackPosition() const { return ParentThing->GetPosition(); }
};

int main()
{
    CThing t;
    t.m_position.X = 10; t.m_position.Y = -20; t.m_position.Z = 30;
    CTCCameraPointShapeActivationBase cp;
    cp.ParentThing = &t;
    C3DVector r = cp.GetTrackPosition();
    t.m_position.X = 0;
    if (r.X != 10 || r.Y != -20 || r.Z != 30) { std::printf("0075c1e0_TEST FAIL\n"); return 1; }
    std::printf("0075c1e0_TEST PASS\n");
    return 0;
}