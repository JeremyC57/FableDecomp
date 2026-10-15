#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CThing {
    C3DVector m_pos;
    const C3DVector& GetPosition() { return m_pos; }
};

struct CTCGatherPointRadialArea {
    CThing* ParentThing;
    float   GatherRadius;
    long vfunc_28(const C3DVector& position)
    {
        CThing* owner = ParentThing;
        float radius = GatherRadius;
        const C3DVector& centre = owner->GetPosition();
        float dx = position.X - centre.X;
        float dy = position.Y - centre.Y;
        if (dx * dx + dy * dy < radius * radius)
            return 1;
        return 0;
    }
};

int main()
{
    CThing t; t.m_pos.X = 10.0f; t.m_pos.Y = 20.0f; t.m_pos.Z = 0.0f;
    CTCGatherPointRadialArea a; a.ParentThing = &t; a.GatherRadius = 5.0f;
    C3DVector in = { 13.0f, 23.0f, 1000.0f };     // distance^2 18 < 25, Z ignored
    C3DVector edge = { 15.0f, 20.0f, 0.0f };      // exactly on the radius: strict test
    C3DVector out = { 16.0f, 20.0f, 0.0f };
    if (a.vfunc_28(in) != 1 || a.vfunc_28(edge) != 0 || a.vfunc_28(out) != 0) { std::printf("007ffd60_TEST FAIL\n"); return 1; }
    std::printf("007ffd60_TEST PASS\n");
    return 0;
}