#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct CThingPlacement {
    C3DVector m_position; int m_updates; long m_lastFlags;
    CThingPlacement() : m_position(0.0f, 0.0f, 0.0f), m_updates(0), m_lastFlags(-1) { }
    virtual ~CThingPlacement() { }
    virtual void SetPosition(const C3DVector& p) { m_position.X = p.X; m_position.Y = p.Y; m_position.Z = p.Z; }
    virtual void Update(long flags) { ++m_updates; m_lastFlags = flags; }
};
class CThing { public: CThingPlacement* m_pPlacement; };
class CWorldMap { public: float GetGroundSizeZAt(const C3DVector& p) { return p.X + p.Y; } };
static CWorldMap g_map;
struct CTCShotDefinitionTearInGround {
    CThing* ParentThing;
    CWorldMap* GetWorldMap() { return &g_map; }
    void UpdateMovement()
    {
        ParentThing->m_pPlacement->Update(0);
        C3DVector position = ParentThing->m_pPlacement->m_position;
        position.Z = GetWorldMap()->GetGroundSizeZAt(position);
        ParentThing->m_pPlacement->SetPosition(position);
    }
};
int main()
{
    CThingPlacement place; place.m_position = C3DVector(1.0f, 2.0f, 9.0f);
    CThing thing; thing.m_pPlacement = &place;
    CTCShotDefinitionTearInGround shot; shot.ParentThing = &thing;
    shot.UpdateMovement();
    if (place.m_updates != 1 || place.m_lastFlags != 0 || place.m_position.X != 1.0f || place.m_position.Y != 2.0f || place.m_position.Z != 3.0f) { std::printf("007bbdca_TEST FAIL\n"); return 1; }
    std::printf("007bbdca_TEST PASS\n");
    return 0;
}