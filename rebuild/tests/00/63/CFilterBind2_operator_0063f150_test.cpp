#include <cstdio>
// Behaviour model: CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> over a CThingPhysical whose
// physics component may report HasNavigationLayers, a CanBlockNavigatingCreature answer, an optional physics mesh and a bounding box;
// the mesh object is a temporary whose destruction is counted.
class C3DVector { public: float X, Y, Z; };
static int g_meshObjects = 0;
class CPhysicsMeshObject { public:
    float MinX, MaxX;
    CPhysicsMeshObject(float a, float b) : MinX(a), MaxX(b) { ++g_meshObjects; }
    CPhysicsMeshObject(const CPhysicsMeshObject& o) : MinX(o.MinX), MaxX(o.MaxX) { ++g_meshObjects; }
    ~CPhysicsMeshObject() { --g_meshObjects; }
    bool IsPointContainedInBoundingBox(const C3DVector& p) const { return p.X >= MinX && p.X <= MaxX; }
};
class CTCPhysicsBase { public:
    bool NavLayers; bool HasMesh; float MinX, MaxX;
    bool HasNavigationLayers() const { return NavLayers; }
    const void* GetPhysicsMeshInfo() const { return HasMesh ? this : 0; }
    CPhysicsMeshObject GetPhysicsMeshObject() const { return CPhysicsMeshObject(MinX, MaxX); }
};
class CThing { public:
    bool Blocks; CTCPhysicsBase* PhysicsTC;
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
class CThingPhysical : public CThing { public:
    bool CanBlockNavigatingCreature() const { return Blocks; }
};
template<class First,class Second> class CFilter_Bind2 {
protected:
    const First& F1;
    const Second& F2;
public:
    CFilter_Bind2(const First& a,const Second& b):F1(a),F2(b){}
    template<class T> bool operator()(const T& thing) const { return F1(thing) && F2(thing); }
};
class CThingFilter_HasNavigationInformation { public:
    bool operator()(const CThingPhysical& thing) const
    {
        const CTCPhysicsBase* physics = thing.GetPhysicsTC();
        if (physics && physics->HasNavigationLayers())
            return thing.CanBlockNavigatingCreature();
        return thing.CanBlockNavigatingCreature();
    }
};
class CThingFilter_OverlappingBoundingBox { public:
    C3DVector Pos; float Radius;
    bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.GetPhysicsTC();
        if (physics->GetPhysicsMeshInfo() == 0)
            return false;
        CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
        return mesh.IsPointContainedInBoundingBox(Pos);
    }
};

int main()
{
    CTCPhysicsBase tc = { true, true, 0.0f, 10.0f };
    CThingPhysical t; t.Blocks = true; t.PhysicsTC = &tc;
    CThingFilter_HasNavigationInformation nav;
    CThingFilter_OverlappingBoundingBox box = { { 5.0f, 0.0f, 0.0f }, 0.1f };
    CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> f(nav, box);
    if (!f(t) || g_meshObjects != 0) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    tc.NavLayers = false;
    if (!f(t)) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    box.Pos.X = 20.0f;
    if (f(t) || g_meshObjects != 0) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    box.Pos.X = 5.0f; tc.HasMesh = false;
    if (f(t)) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    tc.HasMesh = true; t.Blocks = false;
    if (f(t)) { std::printf("0063f150_TEST FAIL\n"); return 1; }
    std::printf("0063f150_TEST PASS\n");
    return 0;
}
