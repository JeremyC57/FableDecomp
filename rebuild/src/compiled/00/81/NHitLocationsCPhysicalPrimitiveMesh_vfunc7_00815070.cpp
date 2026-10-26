// NHitLocations::CPhysicalPrimitiveMesh::GetFirstIntersectionWithFiniteLine,
// retail 0x00815070. Isolated main-review fidelity source; author009 remains frozen. PDB identity: egor 19838 / debug 0x0080DF40.
extern float g_MinVectorLengthSquared_0129ba3c;       // retail 0x0129BA3C
extern const float g_NegativeOne_0122dee0;                  // retail 0x0122DEE0
class C3DVector { public:
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
class CThing;
class CThingPhysicsMeshInfo;
class CPhysicsMeshObject;
#define CTCPhysicsBase CTCPhysicsBasePdb
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
// Retail virtual interface overlay; generated donor records data, not virtual methods.
class CTCPhysicsBase { public:
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
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* Physics;                            // +0x60
};
// Original physics component resource description, distinct from CPhysicsMeshInfo.
class CThingPhysicsMeshInfo { public:
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
class CMeshIntersectionData { public:
    C3DVector IntersectionPosition;                 // +0x00
    C3DVector Normal;                               // +0x0c
    float DistanceSquared;                          // +0x18
};
// RTTI0125d084: CPhysicsMeshObject derives CBaseClass at offset0.
class CBaseClass { public: virtual ~CBaseClass(); };
#define CIVCountedPointeeBase CIVCountedPointeeBasePdb
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const;
    mutable unsigned long IVRefCount;
    CIVCountedPointeeBase():IVRefCount(1){}
    void AddRef()const { ++IVRefCount; }
    long Release()const { return (long)--IVRefCount; }
    long GetRefCount()const { return (long)IVRefCount; }
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBasePdb));
template<class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer():Object(0){}
    CIVCountedPointer(const CIVCountedPointer& other):Object(other.Object) { if(Object)Object->AddRef(); }
    void Unassign() { if(Object){if(Object->Release()==0)Object->Delete();Object=0;} }
    CIVCountedPointer& operator=(const CIVCountedPointer& other) {
        if(Object!=other.Object){Unassign();Object=other.Object;if(Object)Object->AddRef();}return *this;
    }
    ~CIVCountedPointer(){Unassign();}
};
class CPhysicsMesh;
class CMatrix3x4 { public:
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
};
class CPhysicsMeshObject : public CBaseClass { public:
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
class CPhysicalPrimitiveBase : public CBaseClass { public:};
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