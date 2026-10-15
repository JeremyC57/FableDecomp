#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CThing {
    C3DVector m_position;
    const C3DVector& GetPosition() const { return m_position; }
};
struct CBaseIntelligentPointer {
    const CThing* PData;
    const CThing* GetPItem() const { return PData; }
    const CThing* operator->() const { return GetPItem(); }
};
struct CTCPreCalculatedNavigationRoute {
    CBaseIntelligentPointer PThingToCalculateRouteTo;
    C3DVector GetRouteDestination() const { return PThingToCalculateRouteTo->GetPosition(); }
};
int main()
{
    CThing t; t.m_position.X = 10.0f; t.m_position.Y = -4.0f; t.m_position.Z = 0.5f;
    CTCPreCalculatedNavigationRoute r; r.PThingToCalculateRouteTo.PData = &t;
    C3DVector d = r.GetRouteDestination();
    if (d.X != 10.0f || d.Y != -4.0f || d.Z != 0.5f) { std::printf("008004d0_TEST FAIL\n"); return 1; }
    std::printf("008004d0_TEST PASS\n");
    return 0;
}