// Behaviour test for CTCSwitchableNavigation::OnCreate (0x00787550): MapIndex = the map index at the thing's position.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
static const C3DVector* g_queried = 0;

struct CNavMapSource {
    virtual ~CNavMapSource() { }
    virtual long GetNavMapIndex(const C3DVector& position) { g_queried = &position; return position.X > 0 ? 7 : 3; }
};
static CNavMapSource g_source;

struct CThing {
    C3DVector m_position;
    const C3DVector& GetPosition() const { return m_position; }
};

struct CTCSwitchableNavigation {
    CThing* ParentThing;
    long    MapIndex;
    CNavMapSource* GetNavMapSource() { return &g_source; }
    void OnCreate();
};

void CTCSwitchableNavigation::OnCreate()
{
    CNavMapSource* source = GetNavMapSource();
    MapIndex = source->GetNavMapIndex(ParentThing->GetPosition());
}

int main()
{
    CThing thing; thing.m_position.X = 1.0f; thing.m_position.Y = 0.0f; thing.m_position.Z = 0.0f;
    CTCSwitchableNavigation n; n.ParentThing = &thing; n.MapIndex = -1;
    n.OnCreate();
    if (n.MapIndex != 7 || g_queried != &thing.m_position) { std::printf("00787550_TEST FAIL\n"); return 1; }
    thing.m_position.X = -1.0f;
    n.OnCreate();
    if (n.MapIndex != 3) { std::printf("00787550_TEST FAIL\n"); return 1; }
    std::printf("00787550_TEST PASS\n");
    return 0;
}
