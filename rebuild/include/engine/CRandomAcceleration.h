#ifndef FABLE_ENGINE_CRANDOMACCELERATION_H
#define FABLE_ENGINE_CRANDOMACCELERATION_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CRandomAcceleration:public CBaseClassNonCopyable {
public:
 static void* FABLE_FASTCALL operator new(unsigned int);
 static void FABLE_FASTCALL operator delete(void*);
 virtual void Delete();
 CRandomAcceleration(unsigned long&);
 virtual ~CRandomAcceleration();
 unsigned char TimeOffsetX,TimeOffsetY,TimeOffsetZ,IndexX,IndexY,IndexZ;
 C3DVector OldOffset;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CRandomAcceleration)==24);
#endif
