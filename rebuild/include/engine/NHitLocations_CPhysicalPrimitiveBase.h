#ifndef FABLE_ENGINE_NHITLOCATIONS_CPHYSICALPRIMITIVEBASE_H
#define FABLE_ENGINE_NHITLOCATIONS_CPHYSICALPRIMITIVEBASE_H
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CThing;class CPersistContext;class CMemoryDataInputStream;class CDataOutputStream;
namespace NHitLocations{enum EPhysicalPrimitiveType;}
namespace NHitLocations{class CPhysicalPrimitiveBase {
public:
 virtual ~CPhysicalPrimitiveBase();
 virtual NHitLocations::CPhysicalPrimitiveBase * Alloc() const = 0;
 virtual CCharString GetToken() const = 0;
 virtual NHitLocations::EPhysicalPrimitiveType GetType() const = 0;
 virtual bool Init(CPersistContext & p0, const CCharString & p1) = 0;
 virtual void TransferBinaryIn(CMemoryDataInputStream & p0) = 0;
 virtual void TransferBinaryOut(CDataOutputStream & p0) const = 0;
 virtual bool GetFirstIntersectionWithFiniteLine(const CThing & p0, const C3DVector & p1, const C3DVector & p2, C3DVector & p3, C3DVector & p4, float p5) const = 0;
 virtual bool GetFirstIntersectionWithTriangle(const CThing & p0, const std::vector<C3DVector,std::allocator<C3DVector> > & p1, C3DVector & p2, C3DVector & p3, float p4) const = 0;
 virtual bool IsPosInside(const CThing & p0, const C3DVector & p1) const = 0;
 virtual C3DVector GetClosestPointToPos2D(const CThing & p0, const C3DVector & p1, C3DVector & p2) const = 0;
 virtual void GetBoundingSphere(const CThing & p0, C3DVector & p1, float & p2) const = 0;
 CPhysicalPrimitiveBase(const NHitLocations::CPhysicalPrimitiveBase & p0);
 CPhysicalPrimitiveBase();
 NHitLocations::CPhysicalPrimitiveBase & operator=(const NHitLocations::CPhysicalPrimitiveBase & p0);
};}
FABLE_STATIC_ASSERT(sizeof(NHitLocations::CPhysicalPrimitiveBase)==4);
#endif
