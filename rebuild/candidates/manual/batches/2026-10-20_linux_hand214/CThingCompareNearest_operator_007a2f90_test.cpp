// behaviour test for CThingCompare_Nearest::operator()<CThingAICreature> (0x007A2F90): null ordering and squared 3D distance compare
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; } };
struct CThing { C3DVector m_pos; CThing(float x, float y, float z) : m_pos(x, y, z) {} const C3DVector& GetPosition() const { return m_pos; } };
struct CThingAICreature : CThing { CThingAICreature(float x, float y, float z) : CThing(x, y, z) {} };
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(float x, float y, float z) : Pos(x, y, z) {}
    template<class T> bool operator()(T* a, T* b) const
    {
        if (!a) return false;
        if (!b) return true;
        C3DVector deltaA = a->GetPosition() - Pos;
        C3DVector deltaB = b->GetPosition() - Pos;
        if (deltaA.GetLengthSquared() < deltaB.GetLengthSquared()) return true;
        return false;
    }
};
int main()
{
    CThingCompare_Nearest c(1.0f, 2.0f, 3.0f);
    CThingAICreature near_(1.0f, 2.0f, 5.0f), far_(4.0f, 2.0f, 3.0f), same(1.0f, 4.0f, 3.0f);   // distances^2: 4, 9, 4
    int bad = 0;
    if (c((CThingAICreature*)0, &near_) || c((CThingAICreature*)0, (CThingAICreature*)0)) bad = 1;   // null a never wins
    if (!c(&far_, (CThingAICreature*)0)) bad = 1;                       // null b always loses
    if (!c(&near_, &far_) || c(&far_, &near_)) bad = 1;
    if (c(&near_, &same) || c(&same, &near_)) bad = 1;                  // equal distance: neither is less
    if (bad) { std::printf("007a2f90_TEST FAIL\n"); return 1; }
    std::printf("007a2f90_TEST PASS\n");
    return 0;
}
