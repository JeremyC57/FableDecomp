#include <cstdio>
struct CThing { int id; };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CTCShotDefinitionBase {
    CThing* m_pParent; bool MultiArrowShotType;
    CTCShotDefinitionBase(CThing* parent) : m_pParent(parent), MultiArrowShotType(false) { }
    virtual ~CTCShotDefinitionBase() { }
};
struct CTCShotDefinitionMultiArrow : CTCShotDefinitionBase {
    C3DVector Acceleration; float MaxCosAngleToTurn; float MaxAngleToTurn; bool StraightArrow;
    CTCShotDefinitionMultiArrow(CThing* parent) : CTCShotDefinitionBase(parent), Acceleration(0.0f, 0.0f, 0.0f), StraightArrow(false)
    {
        MultiArrowShotType = true;
    }
};
int main()
{
    CThing t;
    CTCShotDefinitionMultiArrow s(&t);
    if (s.m_pParent != &t || !s.MultiArrowShotType || s.StraightArrow || s.Acceleration.X != 0.0f || s.Acceleration.Y != 0.0f || s.Acceleration.Z != 0.0f) { std::printf("007bbc65_TEST FAIL\n"); return 1; }
    std::printf("007bbc65_TEST PASS\n");
    return 0;
}