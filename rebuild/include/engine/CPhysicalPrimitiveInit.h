#ifndef FABLE_ENGINE_CPHYSICALPRIMITIVEINIT_H
#define FABLE_ENGINE_CPHYSICALPRIMITIVEINIT_H
#include "engine/NHitLocations_CPhysicalPrimitiveBase.h"
#include "engine/CCountedPointer.h"
class CPhysicalPrimitiveInit {
public:
 bool SetPPrimitiveFromTokenAndParams(CPersistContext & p0, const CCharString & p1, const CCharString & p2);
 bool SetPPrimitiveFromType(NHitLocations::EPhysicalPrimitiveType p0);
 static void FABLE_FASTCALL StatRegisterClass();
 static std::vector<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>,std::allocator<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> > > RegisteredPrimitives;
 CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;
 CPhysicalPrimitiveInit(const CPhysicalPrimitiveInit & p0);
 CPhysicalPrimitiveInit();
 ~CPhysicalPrimitiveInit();
 CPhysicalPrimitiveInit & operator=(const CPhysicalPrimitiveInit & p0);
};
FABLE_STATIC_ASSERT(sizeof(CPhysicalPrimitiveInit)==8);
#endif
