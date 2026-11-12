#include <cstdio>
// Behaviour model: CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> over a CThingPhysical whose
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
template<class TF1,class TF2,class TF3> class CFilter_Bind3 {
protected:
    const TF1& F1;
    const TF2& F2;
    const TF3& F3;
public:
    CFilter_Bind3(const TF1& f1,const TF2& f2,const TF3& f3):F1(f1),F2(f2),F3(f3){}
    template<class T> bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t);}
};
class CThingFilter_IsNot { public:
    const CThing& Thing;
    CThingFilter_IsNot(const CThing& thing) : Thing(thing) {}
    bool operator()(const CThing& thing) const { return &thing != &Thing; }
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
    CTCPhysicsBase tc = { false, true, 0.0f, 10.0f };
    CThingPhysical a; a.Blocks = true; a.PhysicsTC = &tc;
    CThingPhysical b; b.Blocks = true; b.PhysicsTC = &tc;
    CThingFilter_IsNot notA(a);
    CThingFilter_HasNavigationInformation nav;
    CThingFilter_OverlappingBoundingBox box = { { 5.0f, 0.0f, 0.0f }, 0.1f };
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> f(notA, nav, box);
    if (f(a) || !f(b) || g_meshObjects != 0) { std::printf("0063f1e0_TEST FAIL\n"); return 1; }
    tc.NavLayers = true;
    if (!f(b)) { std::printf("0063f1e0_TEST FAIL\n"); return 1; }
    box.Pos.X = -1.0f;
    if (f(b) || g_meshObjects != 0) { std::printf("0063f1e0_TEST FAIL\n"); return 1; }
    box.Pos.X = 5.0f; b.Blocks = false;
    if (f(b)) { std::printf("0063f1e0_TEST FAIL\n"); return 1; }
    std::printf("0063f1e0_TEST PASS\n");
    return 0;
}
