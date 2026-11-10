#include <stdio.h>
#include <assert.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define FABLE_STATIC_ASSERT(x)
#else
#define FABLE_JOIN2(x,y) x##y
#define FABLE_JOIN(x,y) FABLE_JOIN2(x,y)
#define FABLE_STATIC_ASSERT(x) typedef char FABLE_JOIN(check_,__LINE__)[(x)?1:-1]
#endif
// Standalone dependencies preserve the exact API bodies used by the selected row.
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
};
class C3DBoundingSphere {
public:
    C3DBoundingSphere():Center(0.0f,0.0f,0.0f),Radius(0.0f){}
    const C3DVector GetCenter() const { return Center; }
    float GetRadius() const { return Radius; }
    C3DVector Center;
    float Radius;
};
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const {}
    void AddRef() const { ++IVRefCount; }
    long Release() const { return --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    mutable unsigned long IVRefCount;
};
#ifndef FABLE_ENGINE_CPHYSICSMESH_H
#define FABLE_ENGINE_CPHYSICSMESH_H
// Retail model (Codex 2026-11-10): both original PDB class records identify
// every member below. Native 00a17620 reads BoundingSphere at0x74 and Radius at0x80.
// Composite members unused by this row retain their complete native storage.
class CPhysicsMesh : public CIVCountedPointeeBase {
public:
    virtual ~CPhysicsMesh();
    const C3DBoundingSphere& GetBoundingSphere() const { return BoundingSphere; }
    bool HasBoundingSphereIntersectedFromTo(float sphere_radius,
        const C3DVector& from, const C3DVector& to) const;
private:
    unsigned long ReferenceCount;                     //0x08
    bool NavigationLineListInfo;                       //0x0c
    unsigned char _pad_0d[3];
    unsigned char HelperPoints[16];                    //0x10 CVectorMap<CCharString,C3DHelperPoint,CCompareString<C3DHelperPoint> >
    unsigned char DummyObjects[16];                    //0x20 CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >
    unsigned char TotalFaceIndices[12];                //0x30 CArray<C3DTriangle2>
    unsigned char TotalPoints[12];                     //0x3c CArray<C3DVector>
    unsigned char NavigationLines[12];                 //0x48 CArray<CArray<C2DLineF> >
    unsigned char NavigationContainmentVolumes[16];    //0x54 CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >
    unsigned char ContainmentVolumes[16];              //0x64 CVectorMap<CCharString,C3DMeshVolume,CKeyPairCompareLess<CCharString,C3DMeshVolume> >
    C3DBoundingSphere BoundingSphere;                  //0x74
    unsigned char BoundingBox[24];                    //0x84 C3DBoundingBox
    unsigned char BoundingCircle[12];                 //0x9c CCircle
    C3DVector CentreOfGravity;                         //0xa8
    unsigned char RootSegment[8];                     //0xb4 CCountedPointer<const CIPhysicsFacesSegmentBase>
    class CMovableResourceMemoryPool* MemoryPool;       //0xbc
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMesh)==0xc0);
#endif

// CPhysicsMesh::HasBoundingSphereIntersectedFromTo -- broad-phase sphere test.
// Leaf: ?HasBoundingSphereIntersectedFromTo@CPhysicsMesh@@QBE_NMABVC3DVector@@0@Z
// Retail: 0x00a17620,99 bytes,thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#ifndef _MSC_VER
#define __fastcall
#endif
float __fastcall GFGetShortestDistanceSquaredBetweenPointAndLine(
    const C3DVector&,const C3DVector&,const C3DVector&,float&);
bool CPhysicsMesh::HasBoundingSphereIntersectedFromTo(float sphere_radius,
    const C3DVector& from,const C3DVector& to) const
{
    const float bonus = 15.0f;
    const C3DVector& sphere_centre = GetBoundingSphere().GetCenter();
    const float this_radius = GetBoundingSphere().GetRadius();
    float along;
    float distance = GFGetShortestDistanceSquaredBetweenPointAndLine(from,to,sphere_centre,along);
    float radius = this_radius + sphere_radius + bonus;
    if (distance <= radius * radius) return true;
    return false;
}

CPhysicsMesh::~CPhysicsMesh() {}
static float supplied_distance;
static const C3DVector* expected_from;
static const C3DVector* expected_to;
static C3DVector expected_center;
static int calls;
float __fastcall GFGetShortestDistanceSquaredBetweenPointAndLine(
    const C3DVector& from,const C3DVector& to,const C3DVector& point,float& along)
{
    assert(&from==expected_from && &to==expected_to);
    assert(point.X==expected_center.X && point.Y==expected_center.Y && point.Z==expected_center.Z);
    along=0.5f; ++calls; return supplied_distance;
}
int main()
{
    CPhysicsMesh mesh;
    C3DBoundingSphere& sphere=const_cast<C3DBoundingSphere&>(mesh.GetBoundingSphere());
    C3DVector from(1.0f,2.0f,3.0f),to(4.0f,6.0f,8.0f);
    expected_from=&from; expected_to=&to;
    expected_center=C3DVector(16.0f,32.0f,64.0f); sphere.Center=expected_center;
    for(int radius=-20;radius<=20;++radius)
    for(int input=-20;input<=20;++input)
    {
        sphere.Radius=(float)radius;
        const float total=(float)(radius+input+15);
        const float square=total*total;
        const float values[5]={-1.0f,0.0f,square-1.0f,square,square+1.0f};
        for(int j=0;j<5;++j)
        {
            supplied_distance=values[j]; calls=0;
            const bool result=mesh.HasBoundingSphereIntersectedFromTo((float)input,from,to);
            assert(result==(supplied_distance<=square)); assert(calls==1);
        }
    }
    unsigned int nan_bits=0x7fc00000u;
    memcpy(&supplied_distance,&nan_bits,4);
    assert(!mesh.HasBoundingSphereIntersectedFromTo(1.0f,from,to));
    puts("00a17620_TEST PASS");
    return 0;
}