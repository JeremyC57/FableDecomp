#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>

#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/C3DVector.h"

class CRightHandedSet {
    C3DVector up, forward;
public:
    CRightHandedSet() : up(0,0,1), forward(0,1,0) { }
    const C3DVector& GetUp() const { return up; }
    const C3DVector& GetForward() const { return forward; }
    C3DVector GetRight() const
    {
        return C3DVector(forward.Y*up.Z-forward.Z*up.Y,
                         forward.Z*up.X-forward.X*up.Z,
                         forward.X*up.Y-forward.Y*up.X);
    }
};

class CMatrix3x4 {
public:
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
    void SetTranslateOnly(const C3DVector& v) { E41=v.X; E42=v.Y; E43=v.Z; }
    void PreScale(float s)
    {
        E11*=s; E12*=s; E13*=s;
        E21*=s; E22*=s; E23*=s;
        E31*=s; E32*=s; E33*=s;
    }
    void SetInverse(const CMatrix3x4& m)
    {
        const float a=m.E11*m.E11+m.E12*m.E12+m.E13*m.E13;
        assert(a>0);
        E11=m.E11/a; E12=m.E21/a; E13=m.E31/a;
        E21=m.E12/a; E22=m.E22/a; E23=m.E32/a;
        E31=m.E13/a; E32=m.E23/a; E33=m.E33/a;
        E41=-(m.E41*E11+m.E42*E21+m.E43*E31);
        E42=-(m.E41*E12+m.E42*E22+m.E43*E32);
        E43=-(m.E41*E13+m.E42*E23+m.E43*E33);
    }
};

struct CThingPhysicsMeshInfo { float RenderSizeX; };
struct C3DBoundingBox { float near_edge; };
struct CPhysicsMesh {
    C3DBoundingBox bounds;
    const C3DBoundingBox& GetBoundingBox() const { return bounds; }
};
struct CPhysicsMeshObject {
    CPhysicsMesh* mesh;
    CPhysicsMesh* GetMesh() const { return mesh; }
};
struct CThingBuildingDef { bool HasWeatherMask; };
struct Physics {
    CThingPhysicsMeshInfo mesh_info;
    CRightHandedSet rhset;
    C3DVector position;
    CPhysicsMesh mesh;
    unsigned missing_on;
    unsigned queries;
    Physics() : position(2,3,4),missing_on(0),queries(0)
    {
        mesh_info.RenderSizeX=50.0f;
        mesh.bounds.near_edge=7.0f;
    }
    const CThingPhysicsMeshInfo* GetPhysicsMeshInfo()
    {
        ++queries;
        return queries==missing_on ? 0 : &mesh_info;
    }
    void GetBoundingInfo(C3DVector& centre,float& radius)
    {
        centre=C3DVector(1,2,3);
        radius=8.0f;
    }
    const CRightHandedSet& GetRHSet() const { return rhset; }
    const C3DVector& GetPosition() const { return position; }
    CPhysicsMeshObject GetPhysicsMeshObject()
    {
        CPhysicsMeshObject object={&mesh};
        return object;
    }
};
struct CThingBuilding {
    unsigned id;
    Physics physics;
    long GetDefGlobalIndex() const { return id; }
    Physics* GetPhysicsTC() { return &physics; }
};
template<class T> struct CDefPointer {
    const T* object;
    const T* operator->() const { return object; }
};
struct DefinitionFixture {
    CThingBuildingDef defs[4];
    void GetDef(long index, CDefPointer<const CThingBuildingDef>* out)
    {
        assert(index>=0 && index<4);
        out->object=&defs[index];
    }
};
struct CThingFilter_None { };
struct SearchFixture {
    CThingBuilding* candidates[4];
    unsigned count, searches;
    template<class T>
    void GetAllLeafThings(const CThingFilter_None&,std::vector<T*>& result)
    {
        ++searches;
        for(unsigned i=0;i<count;++i)
            result.push_back(candidates[i]);
    }
};
struct CEnvironment {
    bool enabled;
    bool IsEnvironmentRequiringWeatherMasks() const { return enabled; }
};
struct C3DBoundingSphere {
    C3DVector centre;
    float radius;
    C3DBoundingSphere(const C3DVector& c,float r) : centre(c),radius(r) { }
};
enum EEngineInterpolationSet { ES_NORMAL };
struct CEnginePrimitiveWeatherMask {
    C3DBoundingSphere sphere;
    C3DBoundingBox box;
    CMatrix3x4 transform;
    float scale;
    CEnginePrimitiveWeatherMask(const C3DBoundingSphere& s,const C3DBoundingBox& b,const CMatrix3x4& xf,
                                float size,int zero,EEngineInterpolationSet set) : sphere(s),box(b),transform(xf),scale(size)
    {
        assert(zero==0 && set==ES_NORMAL);
    }
};
struct EngineFixture {
    unsigned primitives;
    float scale[2], x[2];
    EngineFixture() : primitives(0) { }
    void SetPersistentPrimitive(int& handle,const CEnginePrimitiveWeatherMask& p,int zero)
    {
        assert(primitives<2 && !zero && p.sphere.radius==8.0f && p.box.near_edge==7.0f);
        handle=static_cast<int>(primitives)+5;
        scale[primitives]=p.scale;
        x[primitives]=p.transform.E41;
        ++primitives;
    }
};
struct CEnginePrimitivePrepareInfo { EngineFixture* Engine; };
struct ManagerFixture {
    unsigned frames;
    void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&) { ++frames; }
};
struct CameraFixture {
    unsigned frames;
    void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { ++frames; }
};
namespace NParticleEngine {
struct ParticleFixture {
    unsigned frames;
    void PreparePrimitives(const CEnginePrimitivePrepareInfo&) { ++frames; }
};
static ParticleFixture fixture;
static bool rendering;
static bool IsRenderingEnabled() { return rendering; }
static ParticleFixture& GetParticleEmitterManager() { return fixture; }
}
class CWorld {
public:
    bool WeatherMaskingPrimitivesSent;
    CEnvironment* PEnvironment_ptr;
    SearchFixture* PThingSearchTools_ptr;
    DefinitionFixture* DefinitionManager;
    ManagerFixture* PThingManager_ptr;
    CameraFixture* PCurrentGameCamera;
    std::vector<int> WeatherMaskingPrimitiveHandles;
    void PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo&);
};

// Selected PreparePrimitivesForEngine body.

// The selected implementation is reproduced verbatim for this standalone fixture.
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)
{
    if (!WeatherMaskingPrimitivesSent && PEnvironment_ptr->IsEnvironmentRequiringWeatherMasks()) {
        std::vector<CThingBuilding*> buildings;
        PThingSearchTools_ptr->GetAllLeafThings<CThingBuilding>(CThingFilter_None(), buildings);
        WeatherMaskingPrimitiveHandles.resize(buildings.size());
        for (long c1 = 0; c1 < static_cast<long>(buildings.size()); ++c1) {
            CDefPointer<const CThingBuildingDef> pdef;
            DefinitionManager->GetDef(buildings[c1]->GetDefGlobalIndex(), &pdef);
            if (pdef->HasWeatherMask) {
                if (buildings[c1]->GetPhysicsTC()->GetPhysicsMeshInfo()) {
                    C3DVector bounding_sphere_centre;
                    float bounding_sphere_radius = 0.0f;
                    buildings[c1]->GetPhysicsTC()->GetBoundingInfo(bounding_sphere_centre, bounding_sphere_radius);
                    const CRightHandedSet& rhset = buildings[c1]->GetPhysicsTC()->GetRHSet();
                    C3DVector up = rhset.GetUp();
                    C3DVector forward = rhset.GetForward();
                    C3DVector right = rhset.GetRight();
                    float scale = 1.0f;
                    const CThingPhysicsMeshInfo* pinfo = buildings[c1]->GetPhysicsTC()->GetPhysicsMeshInfo();
                    if (pinfo)
                        scale = pinfo->RenderSizeX;
                    CMatrix3x4 transform;
                    transform.SetTranslateOnly(buildings[c1]->GetPhysicsTC()->GetPosition());
                    transform.E11 = -right.X; transform.E12 = -right.Y; transform.E13 = -right.Z;
                    transform.E21 = -forward.X; transform.E22 = -forward.Y; transform.E23 = -forward.Z;
                    transform.E31 = up.X; transform.E32 = up.Y; transform.E33 = up.Z;
                    transform.PreScale(scale * 0.01f);
                    CMatrix3x4 inv_transform;
                    inv_transform.SetInverse(transform);
                    C3DBoundingBox bbox = buildings[c1]->GetPhysicsTC()->GetPhysicsMeshObject().GetMesh()->GetBoundingBox();
                    CEnginePrimitiveWeatherMask weather_mask_primitive(C3DBoundingSphere(bounding_sphere_centre, bounding_sphere_radius), bbox, inv_transform, scale, 0, (EEngineInterpolationSet)0);
                    info.Engine->SetPersistentPrimitive(WeatherMaskingPrimitiveHandles[c1], weather_mask_primitive, 0);
                }
            }
        }
        WeatherMaskingPrimitivesSent = true;
    }
    PThingManager_ptr->PreparePrimitivesForEngine(info);
    if (NParticleEngine::IsRenderingEnabled())
        NParticleEngine::GetParticleEmitterManager().PreparePrimitives(info);
    PCurrentGameCamera->PreparePrimitivesForRendering(info);
}


int main()
{
    CWorld world;
    CEnvironment env={false};
    SearchFixture search={ };
    DefinitionFixture definitions={ };
    ManagerFixture manager={0};
    CameraFixture camera={0};
    EngineFixture engine;
    CEnginePrimitivePrepareInfo info={&engine};
    CThingBuilding buildings[4];
    for (unsigned i=0;i<4;++i) {
        buildings[i].id=i;
        definitions.defs[i].HasWeatherMask=i!=0;
        search.candidates[i]=buildings+i;
    }
    search.count=4;
    buildings[1].physics.missing_on=1;
    buildings[2].physics.missing_on=2;
    world.WeatherMaskingPrimitivesSent=false;
    world.PEnvironment_ptr=&env;
    world.PThingSearchTools_ptr=&search;
    world.DefinitionManager=&definitions;
    world.PThingManager_ptr=&manager;
    world.PCurrentGameCamera=&camera;
    world.PreparePrimitivesForEngine(info);
    assert(!search.searches && !engine.primitives && !world.WeatherMaskingPrimitivesSent);
    assert(!NParticleEngine::fixture.frames && manager.frames==1 && camera.frames==1);
    env.enabled=true;
    NParticleEngine::rendering=true;
    world.PreparePrimitivesForEngine(info);
    world.PreparePrimitivesForEngine(info);
    assert(search.searches==1 && engine.primitives==2 && world.WeatherMaskingPrimitivesSent);
    assert(world.WeatherMaskingPrimitiveHandles.size()==4);
    assert(world.WeatherMaskingPrimitiveHandles[0]==0 && world.WeatherMaskingPrimitiveHandles[1]==0);
    assert(world.WeatherMaskingPrimitiveHandles[2]==5 && world.WeatherMaskingPrimitiveHandles[3]==6);
    assert(engine.scale[0]==1.0f && engine.scale[1]==50.0f);
    assert(std::fabs(engine.x[0]-200.0f)<0.01f && std::fabs(engine.x[1]-4.0f)<0.01f);
    assert(NParticleEngine::fixture.frames==2 && manager.frames==3 && camera.frames==3);
    puts("004a1cb0_TEST PASS");
    return 0;
}
