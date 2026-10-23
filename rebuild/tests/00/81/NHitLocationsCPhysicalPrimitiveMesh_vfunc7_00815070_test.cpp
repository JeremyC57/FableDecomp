#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 75e0bd0ce4aa57debab8a7aa71c43077c54e4ff9909da719d7defe32cbb1d5d2
// NHitLocations::CPhysicalPrimitiveMesh::GetFirstIntersectionWithFiniteLine,
// retail 0x00815070. Isolated main-review fidelity source; author009 remains frozen. PDB identity: egor 19838 / debug 0x0080DF40.
extern float g_MinVectorLengthSquared_0129ba3c;       // retail 0x0129BA3C
extern const float g_NegativeOne_0122dee0;                  // retail 0x0122DEE0
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}
    const C3DVector operator-(const C3DVector& other) const {
        return C3DVector(X-other.X,Y-other.Y,Z-other.Z);
    }
    __forceinline const C3DVector operator-() const {
        return C3DVector(X*-1.0f,Y*-1.0f,Z*-1.0f);
    }
    float SquaredLength() const { return X * X + (Y * Y + Z * Z); }
};
struct CThing;
struct CThingPhysicsMeshInfo;
struct CPhysicsMeshObject;
#define CTCPhysicsBase CTCPhysicsBasePdb
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
// Retail virtual interface overlay; generated donor records data, not virtual methods.
struct CTCPhysicsBase {
#define PHYSICS_SLOT(n) virtual void PhysicsSlot##n() {}
    virtual ~CTCPhysicsBase() {}
    PHYSICS_SLOT(1) PHYSICS_SLOT(2) PHYSICS_SLOT(3)
    PHYSICS_SLOT(4) PHYSICS_SLOT(5) PHYSICS_SLOT(6) PHYSICS_SLOT(7)
    PHYSICS_SLOT(8) PHYSICS_SLOT(9) PHYSICS_SLOT(10) PHYSICS_SLOT(11)
    PHYSICS_SLOT(12) PHYSICS_SLOT(13) PHYSICS_SLOT(14) PHYSICS_SLOT(15)
    PHYSICS_SLOT(16) PHYSICS_SLOT(17) PHYSICS_SLOT(18) PHYSICS_SLOT(19)
    PHYSICS_SLOT(20) PHYSICS_SLOT(21) PHYSICS_SLOT(22) PHYSICS_SLOT(23)
    PHYSICS_SLOT(24) PHYSICS_SLOT(25) PHYSICS_SLOT(26) PHYSICS_SLOT(27)
    PHYSICS_SLOT(28) PHYSICS_SLOT(29) PHYSICS_SLOT(30) PHYSICS_SLOT(31)
    PHYSICS_SLOT(32) PHYSICS_SLOT(33) PHYSICS_SLOT(34) PHYSICS_SLOT(35)
    PHYSICS_SLOT(36) PHYSICS_SLOT(37) PHYSICS_SLOT(38) PHYSICS_SLOT(39)
    PHYSICS_SLOT(40) PHYSICS_SLOT(41) PHYSICS_SLOT(42) PHYSICS_SLOT(43)
    PHYSICS_SLOT(44) PHYSICS_SLOT(45) PHYSICS_SLOT(46) PHYSICS_SLOT(47)
    PHYSICS_SLOT(48) PHYSICS_SLOT(49) PHYSICS_SLOT(50) PHYSICS_SLOT(51)
    PHYSICS_SLOT(52) PHYSICS_SLOT(53) PHYSICS_SLOT(54) PHYSICS_SLOT(55)
    PHYSICS_SLOT(56) PHYSICS_SLOT(57) PHYSICS_SLOT(58) PHYSICS_SLOT(59)
    PHYSICS_SLOT(60) PHYSICS_SLOT(61) PHYSICS_SLOT(62) PHYSICS_SLOT(63)
    PHYSICS_SLOT(64) PHYSICS_SLOT(65) PHYSICS_SLOT(66) PHYSICS_SLOT(67)
    PHYSICS_SLOT(68) PHYSICS_SLOT(69) PHYSICS_SLOT(70) PHYSICS_SLOT(71)
    PHYSICS_SLOT(72) PHYSICS_SLOT(73) PHYSICS_SLOT(74) PHYSICS_SLOT(75)
    PHYSICS_SLOT(76) PHYSICS_SLOT(77) PHYSICS_SLOT(78) PHYSICS_SLOT(79)
    PHYSICS_SLOT(80) PHYSICS_SLOT(81) PHYSICS_SLOT(82) PHYSICS_SLOT(83)
    PHYSICS_SLOT(84) PHYSICS_SLOT(85) PHYSICS_SLOT(86) PHYSICS_SLOT(87)
    PHYSICS_SLOT(88) PHYSICS_SLOT(89) PHYSICS_SLOT(90) PHYSICS_SLOT(91)
    PHYSICS_SLOT(92)
    CPhysicsMeshObject GetPhysicsMeshObject() const; // 006B0850, original no-argument member
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const; // slot 93 / +0x174, PDB recovered
#undef PHYSICS_SLOT
    CThing* ParentThing;
    unsigned long Unknown08;
    C3DVector Position,OldPosition;
    long OldPositionLastFrameSet;
    C3DVector Velocity;
    float Radius;
    CDefPointeeBase* PhysicsDef_Object;
    unsigned char OldMoving[4];
};
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase)==sizeof(CTCPhysicsBasePdb));
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsBase,Position)==offsetof(CTCPhysicsBasePdb,Position_X));
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* Physics;                            // +0x60
};
// Original physics component resource description, distinct from CPhysicsMeshInfo.
struct CThingPhysicsMeshInfo {
    unsigned short BankIndex;
    float RenderSizeX;
    C3DVector ToCentre;
    float BoundingRadius,BoundingCircleRadius;
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysicsMeshInfo)==28);
namespace NBBBMaths { struct CSphere {
    float Radius;
    C3DVector Base;
    CSphere(const C3DVector& center,float radius) : Radius(radius),Base(center) {}
    bool GetFirstIntersectionWithFiniteLine(const C3DVector& from,
                                            const C3DVector& to,
                                            C3DVector& pos,
                                            C3DVector& normal) const; // 0x00A58790
}; }
struct CMeshIntersectionData {
    C3DVector IntersectionPosition;                 // +0x00
    C3DVector Normal;                               // +0x0c
    float DistanceSquared;                          // +0x18
};
// RTTI0125d084: CPhysicsMeshObject derives CBaseClass at offset0.
struct CBaseClass { virtual ~CBaseClass(); };
#define CIVCountedPointeeBase CIVCountedPointeeBasePdb
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const;
    mutable unsigned long IVRefCount;
    CIVCountedPointeeBase():IVRefCount(1){}
    void AddRef()const { ++IVRefCount; }
    long Release()const { return (long)--IVRefCount; }
    long GetRefCount()const { return (long)IVRefCount; }
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBasePdb));
template<class T> struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer():Object(0){}
    CIVCountedPointer(const CIVCountedPointer& other):Object(other.Object) { if(Object)Object->AddRef(); }
    void Unassign() { if(Object){if(Object->Release()==0)Object->Delete();Object=0;} }
    CIVCountedPointer& operator=(const CIVCountedPointer& other) {
        if(Object!=other.Object){Unassign();Object=other.Object;if(Object)Object->AddRef();}return *this;
    }
    ~CIVCountedPointer(){Unassign();}
};
struct CPhysicsMesh;
struct CMatrix3x4 {
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
};
struct CPhysicsMeshObject : CBaseClass {
    CIVCountedPointer<const CPhysicsMesh> Mesh;
    CMatrix3x4 PositionMatrix;
    float Scale,OO_Scale;
    CPhysicsMeshObject();
    CPhysicsMeshObject(const CPhysicsMeshObject& other)
      :CBaseClass(other),Mesh(other.Mesh),PositionMatrix(other.PositionMatrix),Scale(other.Scale),OO_Scale(other.OO_Scale){}
    virtual ~CPhysicsMeshObject(); // originalUAE, retail00a127d0
    bool GetPointIntersectionFromTo(const C3DVector& from,const C3DVector& to,
                                    CMeshIntersectionData& hit)const;
};
FABLE_STATIC_ASSERT(sizeof(CMatrix3x4)==48);
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshObject)==64);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject,Mesh)==4);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject,PositionMatrix)==8);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshObject,Scale)==0x38);
FABLE_STATIC_ASSERT(sizeof(CMeshIntersectionData)==28);
namespace NHitLocations {
struct CPhysicalPrimitiveBase : CBaseClass {};
struct CPhysicalPrimitiveMesh : CPhysicalPrimitiveBase {
    virtual ~CPhysicalPrimitiveMesh() {}
    virtual void MeshSlot1() {}
    virtual void MeshSlot2() {}
    virtual void MeshSlot3() {}
    virtual void MeshSlot4() {}
    virtual void MeshSlot5() {}
    virtual void MeshSlot6() {}
    virtual bool GetFirstIntersectionWithFiniteLine(
        const CThing& thing, const C3DVector& from, const C3DVector& to,
        C3DVector& outPosition, C3DVector& outNormal, float unused) const;
    virtual void MeshSlot8() {}
    virtual void MeshSlot9() {}
    virtual void MeshSlot10() {}
    virtual void GetBoundingSphere(const CThing& thing, C3DVector& center,
                                   float& radius) const; // slot 11 / +0x2c
};
}

bool NHitLocations::CPhysicalPrimitiveMesh::GetFirstIntersectionWithFiniteLine(
    const CThing& thing, const C3DVector& from, const C3DVector& to,
    C3DVector& outPosition, C3DVector& outNormal, float unused) const
{
    C3DVector line = to - from;
    if (!(line.SquaredLength() >
          g_MinVectorLengthSquared_0129ba3c *
          g_MinVectorLengthSquared_0129ba3c))
        return false;

    CTCPhysicsBase* physics = thing.Physics;
    if (!physics || !physics->GetPhysicsMeshInfo())
        return false;

    C3DVector center;
    float radius;
    GetBoundingSphere(thing, center, radius);
    C3DVector dummyPosition;
    C3DVector dummyNormal;
    if (!NBBBMaths::CSphere(center,radius).GetFirstIntersectionWithFiniteLine(from, to,
                                                    dummyPosition, dummyNormal))
        return false;

    CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
    CMeshIntersectionData hit;
    if (mesh.GetPointIntersectionFromTo(from, to, hit)) {
        outPosition = hit.IntersectionPosition;
        outNormal = -hit.Normal;
        return true;
    }
    return false;
}

// END exact source snapshot
float g_MinVectorLengthSquared_0129ba3c=0.125f;
const float g_NegativeOne_0122dee0=-1.0f;
static int InfoCalls,BoundCalls,SphereCalls,ObjectCalls,QueryCalls,Destroyed,Copies,Deletes;
static bool InfoAvailable=true,SphereHit=true,MeshHit=true,ContractFailure=false;
static const CThing* ExpectedThing;
static const C3DVector* ExpectedFrom;
static const C3DVector* ExpectedTo;
static CThingPhysicsMeshInfo MeshInfo;
// The mesh-provider mock supplies the actual type-erased CIV pointee interface.
// This leaf never dereferences CPhysicsMesh itself; geometry is an external call.
struct MockMeshOwner : CIVCountedPointeeBase {
    mutable bool Deleted;
    MockMeshOwner():Deleted(false){}
    virtual void Delete()const {++Deletes;Deleted=true;}
};
static MockMeshOwner MeshOwner;
CBaseClass::~CBaseClass(){}
void CIVCountedPointeeBase::Delete()const {delete this;}
const CThingPhysicsMeshInfo* CTCPhysicsBase::GetPhysicsMeshInfo()const {
    ++InfoCalls;
    return InfoAvailable?&MeshInfo:0;
}
void NHitLocations::CPhysicalPrimitiveMesh::GetBoundingSphere(const CThing& thing,C3DVector& center,float& radius)const {
    ++BoundCalls;
    if(&thing!=ExpectedThing)ContractFailure=true;
    center=MeshInfo.ToCentre;radius=MeshInfo.BoundingRadius;
}
bool NBBBMaths::CSphere::GetFirstIntersectionWithFiniteLine(const C3DVector& from,const C3DVector& to,C3DVector& pos,C3DVector& normal)const {
    ++SphereCalls;
    if(&from!=ExpectedFrom||&to!=ExpectedTo||Radius!=5.0f||Base.X!=2.0f||Base.Y!=3.0f||Base.Z!=4.0f)ContractFailure=true;
    pos=C3DVector(99.0f,98.0f,97.0f);normal=C3DVector(96.0f,95.0f,94.0f);
    return SphereHit;
}
CPhysicsMeshObject::CPhysicsMeshObject():Scale(2.0f),OO_Scale(0.5f) {
    PositionMatrix.E11=1;PositionMatrix.E12=2;PositionMatrix.E13=3;
    PositionMatrix.E21=4;PositionMatrix.E22=5;PositionMatrix.E23=6;
    PositionMatrix.E31=7;PositionMatrix.E32=8;PositionMatrix.E33=9;
    PositionMatrix.E41=10;PositionMatrix.E42=11;PositionMatrix.E43=12;
}
CPhysicsMeshObject::~CPhysicsMeshObject(){++Destroyed;}
CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject()const {
    ++ObjectCalls;
    CPhysicsMeshObject object;
    object.Mesh.Object=&MeshOwner;
    MeshOwner.AddRef();
    return object;
}
bool CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector& from,const C3DVector& to,CMeshIntersectionData& hit)const {
    ++QueryCalls;
    if(&from!=ExpectedFrom||&to!=ExpectedTo||Mesh.Object!=&MeshOwner||MeshOwner.IVRefCount!=2||
       PositionMatrix.E11!=1||PositionMatrix.E43!=12||Scale!=2.0f||OO_Scale!=0.5f)ContractFailure=true;
    hit.IntersectionPosition=C3DVector(7,8,9);hit.Normal=C3DVector(.25f,-.5f,.75f);hit.DistanceSquared=11;
    return MeshHit;
}
static void Reset(bool sphere,bool mesh) {
    InfoCalls=BoundCalls=SphereCalls=ObjectCalls=QueryCalls=Destroyed=0;
    ContractFailure=false;InfoAvailable=true;SphereHit=sphere;MeshHit=mesh;
}
static bool Same(const C3DVector& p,float x,float y,float z){return p.X==x&&p.Y==y&&p.Z==z;}
#define CHECK(v) do {if(!(v)){printf("failed line %d\n",__LINE__);return 1;}}while(0)
int main(){
    CHECK(sizeof(CTCPhysicsBase)==0x40&&sizeof(CPhysicsMeshObject)==0x40&&sizeof(CMeshIntersectionData)==28);
    CHECK(offsetof(CPhysicsMeshObject,Mesh)==4&&offsetof(CPhysicsMeshObject,PositionMatrix)==8);
    static NHitLocations::CPhysicalPrimitiveMesh primitive;
    CTCPhysicsBase physics;CThing thing;thing.Physics=&physics;ExpectedThing=&thing;
    MeshInfo.BankIndex=17;MeshInfo.RenderSizeX=2;MeshInfo.ToCentre=C3DVector(2,3,4);MeshInfo.BoundingRadius=5;MeshInfo.BoundingCircleRadius=6;
    C3DVector from(0,0,0),to(10,0,0),position(-1,-2,-3),normal(-4,-5,-6);ExpectedFrom=&from;ExpectedTo=&to;
    Reset(true,true);ExpectedTo=&from;
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,from,position,normal,999));
    CHECK(!InfoCalls&&!BoundCalls&&!SphereCalls&&!ObjectCalls&&Same(position,-1,-2,-3)&&Same(normal,-4,-5,-6));
    C3DVector tiny(.125f,0,0);ExpectedTo=&tiny;
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,tiny,position,normal,999)&&!InfoCalls);
    unsigned long nanBits=0x7fc00000UL;C3DVector bad;memcpy(&bad.X,&nanBits,4);bad.Y=bad.Z=0;ExpectedTo=&bad;
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,bad,position,normal,999)&&!InfoCalls);
    ExpectedTo=&to;thing.Physics=0;
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,to,position,normal,999)&&!InfoCalls);
    thing.Physics=&physics;Reset(true,true);InfoAvailable=false;
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,to,position,normal,999));
    CHECK(InfoCalls==1&&!BoundCalls&&!SphereCalls&&!ObjectCalls&&MeshOwner.IVRefCount==1);
    Reset(false,true);
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,to,position,normal,999));
    CHECK(!ContractFailure&&InfoCalls==1&&BoundCalls==1&&SphereCalls==1&&!ObjectCalls&&!QueryCalls&&!Destroyed);
    Reset(true,false);
    CHECK(!primitive.GetFirstIntersectionWithFiniteLine(thing,from,to,position,normal,999));
    CHECK(!ContractFailure&&ObjectCalls==1&&QueryCalls==1&&Destroyed==2&&MeshOwner.IVRefCount==1);
    CHECK(Same(position,-1,-2,-3)&&Same(normal,-4,-5,-6));
    Reset(true,true);
    CHECK(primitive.GetFirstIntersectionWithFiniteLine(thing,from,to,position,normal,999));
    CHECK(!ContractFailure&&Destroyed==2&&MeshOwner.IVRefCount==1&&!Deletes);
    CHECK(Same(position,7,8,9)&&Same(normal,-.25f,.5f,-.75f));
    // Output aliasing retains the leaf's position-then-normal store order.
    Reset(true,true);
    CHECK(primitive.GetFirstIntersectionWithFiniteLine(thing,from,to,position,position,999));
    CHECK(!ContractFailure&&Same(position,-.25f,.5f,-.75f)&&MeshOwner.IVRefCount==1);
    // Verify actual CIV member release and virtual Delete at last-owner exit.
    {CPhysicsMeshObject first;first.Mesh.Object=&MeshOwner;
     {CPhysicsMeshObject second(first);CHECK(MeshOwner.IVRefCount==2);}
     CHECK(MeshOwner.IVRefCount==1&&!MeshOwner.Deleted);
    }
    CHECK(MeshOwner.IVRefCount==0&&MeshOwner.Deleted&&Deletes==1);
    puts("00815070_TEST PASS");return 0;
}