#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CThing { public:
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
};
class CTCCoopSpirit { public:
    CThing* ParentThing;
    C3DVector GetFocalPos() const
    {
        const C3DVector& position = ParentThing->GetPosition();
        C3DVector focal;
        focal.X = position.X;
        focal.Y = position.Y;
        focal.Z = position.Z + 0.2f;
        return focal;
    }
};
int main()
{
    CThing thing; thing.Position.X = 1.5f; thing.Position.Y = -2.0f; thing.Position.Z = 3.0f;
    CTCCoopSpirit spirit; spirit.ParentThing = &thing;
    C3DVector f = spirit.GetFocalPos();
    if (f.X != 1.5f || f.Y != -2.0f || f.Z != 3.0f + 0.2f || thing.Position.Z != 3.0f) { std::printf("0066ffd0_TEST FAIL\n"); return 1; }
    std::printf("0066ffd0_TEST PASS\n");
    return 0;
}