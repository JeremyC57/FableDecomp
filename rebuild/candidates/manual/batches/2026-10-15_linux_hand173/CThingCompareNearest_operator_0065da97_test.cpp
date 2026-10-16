// behaviour test for CThingCompare_Nearest::operator()<CThingAICreature> (0x0065DA97)
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; } };
struct CThing { C3DVector p; const C3DVector& GetPosition() const { return p; } };
struct CThingCompare_Nearest {
    C3DVector Pos;
    template<class T> T* operator()(T* a, T* b) const
    {
        if (!a) return b;
        if (!b) return a;
        C3DVector offsetA = a->GetPosition() - Pos; C3DVector offsetB = b->GetPosition() - Pos;
        if (offsetA.GetLengthSquared() < offsetB.GetLengthSquared()) return a;
        return b;
    }
};
int main()
{
    CThingCompare_Nearest n; n.Pos = C3DVector(1, 1, 1);
    CThing a; a.p = C3DVector(2, 1, 1); CThing b; b.p = C3DVector(1, 4, 1);
    int bad = n(&a, &b) != &a || n(&b, &a) != &a || n((CThing*)0, &b) != &b || n(&a, (CThing*)0) != &a;
    b.p = a.p; if (n(&a, &b) != &b) bad = 1;
    if (bad) { std::printf("0065da97_TEST FAIL\n"); return 1; }
    std::printf("0065da97_TEST PASS\n");
    return 0;
}
