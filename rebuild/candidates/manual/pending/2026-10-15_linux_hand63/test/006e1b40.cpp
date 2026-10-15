// Behaviour test for CParticleEmitter::GetCurrentOrientation (0x006E1B40): Up = Transform row 2,
// Forward = Transform row 1.
#include <cstdio>

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};

struct CMatrix3x4 {
    C3DVector Row[4];
    C3DVector GetRow(long index) const { return Row[index]; }
};

struct CParticleEmitter {
    virtual void v00() {}
    unsigned char _state[0xac];
    CMatrix3x4    Transform;
    CRightHandedSet GetCurrentOrientation() const;
};

CRightHandedSet CParticleEmitter::GetCurrentOrientation() const
{
    return CRightHandedSet(Transform.GetRow(2), Transform.GetRow(1));
}

int main()
{
    CParticleEmitter e;
    for (int r = 0; r < 4; ++r) {
        e.Transform.Row[r].X = r * 10.0f + 1.0f;
        e.Transform.Row[r].Y = r * 10.0f + 2.0f;
        e.Transform.Row[r].Z = r * 10.0f + 3.0f;
    }
    CRightHandedSet o = e.GetCurrentOrientation();
    if (o.Up.X != 21.0f || o.Up.Y != 22.0f || o.Up.Z != 23.0f
        || o.Forward.X != 11.0f || o.Forward.Y != 12.0f || o.Forward.Z != 13.0f) {
        std::printf("006e1b40_TEST FAIL\n");
        return 1;
    }
    std::printf("006e1b40_TEST PASS\n");
    return 0;
}
