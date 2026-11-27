// 0075aaf0 CTCPhysicsRigidBody::CheckForCollisions
// Leaf: ?CheckForCollisions@CTCPhysicsRigidBody@@AAE?AW4ECollideState@1@AAVCRigidBodyConfig@CRigidBody@@@Z
// Flags:
#define FABLE_RETAIL_RIGIDBODY_CHECK_COLLISIONS_PASS29
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CTCPHYSICSBASE_SEARCHTOOLS
#define FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_EXPLOSIVE_TRAIL_UPDATE_PASS28
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_CPLANE_ACCESSORS

#define FABLE_RETAIL_CRIGIDBODYCONFIG_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_INVENTORY_LONG_OWNERS_PASS27
#define FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#define FABLE_RETAIL_STLP_VECTOR_INDEX_START
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#include "engine/ETCInterfaceType.h"
#include "engine/C2DBoxI.h"
#define private public
#define protected public
#include "engine/CTCPhysicsRigidBody.h"
#undef protected
#undef private
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_CanCollideWithThing.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorldMap.h"
#include "engine/CPlane.h"

inline float FABLE_FASTCALL GFDotProduct(const C3DVector& vector1,const C3DVector& vector2) {
    return vector1.X*vector2.X+vector1.Y*vector2.Y+vector1.Z*vector2.Z;
}

CTCPhysicsRigidBody::ECollideState CTCPhysicsRigidBody::CheckForCollisions(CRigidBody::CRigidBodyConfig& configuration) {
    ECollideState collide_state = CLEAR;
    float depth_epsilon = 0.001f;
    long no_vertices = Body.BodyBoundingVertices.size();
    bool collide_map = true;
    std::vector<CThing*> things;
    GetThingSearchTools().GetAllThingsInRoughArea<CThing>(GetPossibleCollidedWithArea(),
        GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()),CThingFilter_CanCollideWithThing(*this)),things);
    for (long c1=0;c1<no_vertices;++c1) {
        C3DVector current_point = configuration.BoundingVertices[c1];
        C3DVector U = current_point-configuration.CMPosition;
        C3DVector velocity = configuration.CMVelocity+(configuration.AngularVelocity^U);
        if (collide_map) {
            float height_z = GetWorldMap().GetGroundSizeZAt(current_point);
            C3DVector normal = GetWorldMap().GetGroundFaceNormalAt(current_point);
            float distance = C3DVector(current_point.X,current_point.Y,height_z).Dot(normal);
            CPlane collide_plane(normal,-distance);
            float axbyczd = GFDotProduct(current_point,collide_plane.GetNormal()) + collide_plane.GetDistance();
            if (axbyczd < -depth_epsilon) return PENETRATING;
            else if (axbyczd < depth_epsilon) {
                float relative_velocity=GFDotProduct(collide_plane.GetNormal(),velocity);
                if (relative_velocity < 0.0f) {
                    collide_state=COLLIDING;
                    CollisionNormal.push_back(collide_plane.GetNormal());
                    CollidingVertexIndex.push_back(c1);
                }
            }
        }
    }
    return collide_state;
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
template<class T> union Storage { double align; unsigned char data[sizeof(T)]; T* get(){return reinterpret_cast<T*>(data);} };
static Storage<CWorldMap> world;
static Storage<CThingSearchTools> search;
static int scans, queries, normals;
static C3DVector axis;
CWorldMap& CTCPhysicsBase::GetWorldMap(){return *world.get();}
const CThingSearchTools& CTCPhysicsBase::GetThingSearchTools()const{return *search.get();}
float CWorldMap::GetGroundSizeZAt(const C3DVector&)const{++queries;return 0;}
C3DVector CWorldMap::GetGroundFaceNormalAt(const C3DVector&)const{++normals;return axis;}
typedef CFilter_Bind2<CThingFilter_IsNot,CThingFilter_CanCollideWithThing> Filters;
template<class T,class Container,class Filter> void CThingSearchTools::GetAllThingsInRoughArea(
    const C2DBoxI& box,const Filter&,Container& list)const{
    assert(box.TLX==-4&&box.BRY==6&&list.empty());++scans;
}
namespace std{
template<> void vector<C3DVector>::_M_insert_overflow(iterator,const C3DVector&,const __false_type&,size_type,bool){assert(false);}
}
struct SearchArea { virtual C2DBoxI area()const{C2DBoxI b;b.TLX=-4;b.BRY=6;return b;} };
template<class V,class T> void storage(V& v,T* start,int count,int capacity){
    T* p[]={start,start+count,start+capacity};memcpy(&v,p,sizeof(p));
}
int main(){
    Storage<CTCPhysicsRigidBody> raw;memset(raw.data,0,sizeof(raw.data));CTCPhysicsRigidBody* physics=raw.get();
    Storage<CRigidBody::CRigidBodyConfig> state;memset(state.data,0,sizeof(state.data));
    SearchArea shim;void** method;memcpy(&method,&shim,sizeof(method));void* slots[98]={0};slots[97]=method[0];
    void** vt=slots;memcpy(raw.data,&vt,sizeof(vt));unsigned char owner[16];void* op=owner;memcpy(raw.data+4,&op,4);
    C3DVector points[3],out[4];long ix[4];int cases=0;
    const float heights[]={-0.125f,-0.0009765625f,0.0f,0.0009765625f,0.125f};
    for(int n=0;n<=3;++n)for(int k=0;k<5;++k)for(int v=-1;v<2;++v)for(int slope=0;slope<2;++slope){
        axis=C3DVector(slope?0.5f:0,0,1);
        state.get()->CMPosition=C3DVector(0,0,0);state.get()->AngularVelocity=C3DVector(0,0,0);
        state.get()->CMVelocity=C3DVector(0,0,(float)v);
        for(int j=0;j<3;++j)points[j]=C3DVector((float)j,0,heights[(k+j)%5]);
        storage(physics->Body.BodyBoundingVertices,points,n,3);storage(state.get()->BoundingVertices,points,n,3);
        storage(physics->CollisionNormal,out,0,4);storage(physics->CollidingVertexIndex,ix,0,4);
        scans=queries=normals=0;int expected=2,steps=0,count=0;long ids[3];
        for(int i=0;i<n;++i){++steps;float z=points[i].Z;if(z< -0.001f){expected=0;break;}
            if(z<0.001f&&v<0){expected=1;ids[count++]=i;}}
        int result=physics->CheckForCollisions(*state.get());
        assert(result==expected && scans==1 && queries==steps && normals==steps);
        assert((int)physics->CollisionNormal.size()==count && (int)physics->CollidingVertexIndex.size()==count);
        for(int q=0;q<count;++q)assert(ix[q]==ids[q]&&out[q].X==axis.X&&out[q].Y==0&&out[q].Z==1);
        ++cases;
    }
    assert(cases==120);puts("0075aaf0_TEST PASS");return 0;
}