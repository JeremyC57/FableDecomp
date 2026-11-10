#ifndef FABLE_ENGINE_PHYSICALPRIMITIVESPHERE_H
#define FABLE_ENGINE_PHYSICALPRIMITIVESPHERE_H
// Retail opt-in (Codex pass18): original Ego_r types2ea95/2ea98, both PDB
// method name sets and native sphere vtable establish the hierarchy and slots.
#include "engine/C3DVector.h"
#include "engine/CDefString.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "stlport/fable_containers.h"
class CThing; class CPersistContext; class CMemoryDataInputStream; class CDataOutputStream;
namespace NHitLocations {
enum EPhysicalPrimitiveType { PHYSICAL_PRIMITIVE_TYPE_NULL=0,PHYSICAL_PRIMITIVE_TYPE_SPHERE=1,PHYSICAL_PRIMITIVE_TYPE_CYLINDER=2,PHYSICAL_PRIMITIVE_TYPE_MESH=3 };
class CPhysicalPrimitiveBase {
public:
 virtual ~CPhysicalPrimitiveBase();
 virtual CPhysicalPrimitiveBase* Alloc() const=0;
 virtual CCharString GetToken() const=0;
 virtual EPhysicalPrimitiveType GetType() const=0;
 virtual bool Init(CPersistContext&,const CCharString&)=0;
 virtual void TransferBinaryIn(CMemoryDataInputStream&)=0;
 virtual void TransferBinaryOut(CDataOutputStream&) const=0;
 virtual bool GetFirstIntersectionWithFiniteLine(const CThing&,const C3DVector&,const C3DVector&,C3DVector&,C3DVector&,float) const=0;
 virtual bool GetFirstIntersectionWithTriangle(const CThing&,const std::vector<C3DVector>&,C3DVector&,C3DVector&,float) const=0;
 virtual bool IsPosInside(const CThing&,const C3DVector&) const=0;
 virtual C3DVector GetClosestPointToPos2D(const CThing&,const C3DVector&,C3DVector&) const=0;
 virtual void GetBoundingSphere(const CThing&,C3DVector&,float&) const=0;
};
class CPhysicalPrimitiveSphere:public CPhysicalPrimitiveBase {
public:
 virtual ~CPhysicalPrimitiveSphere();
 virtual CPhysicalPrimitiveBase* Alloc() const;
 virtual CCharString GetToken() const;
 virtual EPhysicalPrimitiveType GetType() const;
 virtual bool Init(CPersistContext&,const CCharString&);
 virtual void TransferBinaryIn(CMemoryDataInputStream&);
 virtual void TransferBinaryOut(CDataOutputStream&) const;
 virtual bool GetFirstIntersectionWithFiniteLine(const CThing&,const C3DVector&,const C3DVector&,C3DVector&,C3DVector&,float) const;
 virtual bool GetFirstIntersectionWithTriangle(const CThing&,const std::vector<C3DVector>&,C3DVector&,C3DVector&,float) const;
 virtual bool IsPosInside(const CThing&,const C3DVector&) const;
 virtual void GetBoundingSphere(const CThing&,C3DVector&,float&) const;
 C3DVector GetSphereBase(const CThing&) const;
 float GetSphereRadius(const CThing& thing) const {
  const CTCPhysicsBase* physics=thing.GetPhysicsTC();
  float radius=Radius;
  if(radius<DELTA_NOTIONAL_ZERO){
   radius=physics->GetRadius();
   if(thing.GetTypeFlags()&8)radius=static_cast<const CThingCreatureBase&>(thing).GetCombatRadius();
  }
  return radius;
 }
 virtual C3DVector GetClosestPointToPos2D(const CThing&,const C3DVector&,C3DVector&) const;
private:
 CDefString BaseName;
 float Radius;
};
}
#endif
