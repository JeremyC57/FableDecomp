#ifndef FABLE_ENGINE_CPARTICLESIMULATION_H
#define FABLE_ENGINE_CPARTICLESIMULATION_H
// Retail opt-in (Codex pass 12): complete Ego_r TPI 0x5899f fields, native
// 00ae0070 Size+0x18, Positions+0x1c, and adjacent relocation source agree.
#include "engine/CMovableResource.h"
#include "engine/CCountedPointer.h"
#include "engine/C4DVector.h"
#include "engine/C3DBoundingBox.h"
class CParticleSimulationRenderData;
class CMovableResourceMemoryPool;
class CParticleSimulation:public CMovableResource {
public:
#ifdef FABLE_RETAIL_PARTICLE_SPRINGS
 friend class CParticleProgram;
#endif
 CParticleSimulation();
 CParticleSimulation(const CParticleSimulation&);
 virtual ~CParticleSimulation();
 virtual void RelocateData(long,void*,void*,long);
 unsigned long GetParticleCount()const{return Size;}
 C3DVector GetParticlePosition(unsigned long index)const{return Positions[index].ToC3DVector();}
 const C4DVector* PeekParticlePositions()const{return Positions;}
 void SetParticlePosition(unsigned long index,const C3DVector& vect){Positions[index].Initialise(vect,1.0f);}
 CCountedPointer<CParticleSimulationRenderData> RenderData;
protected:
 float Timestep;
 bool TimestepChanged;
 float TimestepMultiplier;
 unsigned long Size;
 C4DVector* Positions;
 C4DVector* LastPositions;
 C4DVector* SavedPositions;
 C4DVector* Accelerations;
 float* SimulationAlphas;
 float MaximumAdjustment;
 float GravityStrength;
 float WindStrength;
 bool DraggingEnable;
 bool DraggingRotational;
 float DraggingStrength;
 bool AccelerationEnable;
 float GlobalDamping;
private:
 double CurrentTime;
 C3DBoundingBox BoundingBox;
 bool BoundingBoxValid;
 long MemoryHandle;
 CMovableResourceMemoryPool* MemoryPool;
};
FABLE_STATIC_ASSERT(sizeof(CParticleSimulation)==0x80);
#endif
