// CEngineInternalPrimitiveAnimatedMesh::InitialiseSimulation(const C3DMeshInfo*,const C3DMesh2&) -- build cloth simulation once and register it
// Actual headers retained. Mesh owner SimulationInfo at64 is opaque in donor; native primitive array count90/pointer94 vs donor98/9C.
// Native C3DPrimitive2 strideA0 vs donorAC (three arrays each shrink4); cloth array begins20 in both.
// SimulationInfo is native20 vs donor24, cloth simulation180 vs184 (ParticleSims12 vs16).
// Callees: B81E00 info ctor, B81ED0 animated cloth ctor, BA7CF0 counted pointer ctor, B82330 add, B817D0 register.
// BA7CF0 creates12-byte block(count0,destroy4,object8), destroyBA7990 is actual one-argument fastcall wrapper.
// Leaf: InitialiseSimulation
#include <stddef.h>
#include "engine/CIVCountedPointeeBase.h"
#include "engine/_quarantine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CEngineSimulationManager.h"
struct CEngineSimulationBase;
struct SimulationCountInfo {int References;void(__fastcall* Destroy)(CEngineSimulationBase*);CEngineSimulationBase* Object;};
struct SimulationPointer {
 CEngineSimulationBase* Data;SimulationCountInfo* Info;
 SimulationPointer(CEngineSimulationBase*); //00BA7CF0
 ~SimulationPointer(){if(Info){--Info->References;if(Info->References==0){Info->Destroy(Info->Object);delete Info;}}Data=0;Info=0;}
};
struct SimulationArray {SimulationPointer* Start;SimulationPointer* Finish;SimulationPointer* Limit;bool Empty()const{return Start==Finish;}};
// Native CIVCountedPointer pointee: actual unsigned IVRefCount and const Delete slot1 (0042D930).
struct CEnginePrimitiveSimulationInfo:CIVCountedPointeeBase {
 SimulationArray Simulations;
 CEnginePrimitiveSimulationInfo(); //00B81E00
 void AddSimulation(unsigned int,const SimulationPointer&); //00B82330
 typedef void(CEnginePrimitiveSimulationInfo::*ReleaseMethod)() const;
 void Release(){if(--IVRefCount==0)(this->*static_cast<ReleaseMethod*>(__vftable)[1])();}
};
struct SimulationInfoPointer {
 CEnginePrimitiveSimulationInfo* Data;
 void Set(CEnginePrimitiveSimulationInfo* p){if(Data){Data->Release();Data=0;}Data=p;}
};
struct CEngineSimulationManager_Methods:CEngineSimulationManager {void AddSimulationInfo(const SimulationInfoPointer&,bool,bool);}; //00B817D0
extern CEngineSimulationManager_Methods* g_SimulationManager; //01436E68
struct C3DPrimitive2 {unsigned char Prefix[0x20];SimulationArray ClothPrimitives;unsigned char Tail[0xa0-0x2c];};
struct C3DMesh2 {unsigned char Prefix[0x90];unsigned int PrimitiveCount;C3DPrimitive2* Primitives;};
struct CEngineSimulationBase {void* VTable;int RunState;unsigned char SimStepOffset;unsigned char Align[3];};
struct ClothVector {float X,Y,Z;};struct ClothMatrix {ClothVector X,Y,Z,Position;};
struct CEngineClothSimulationAnimatedMesh:CEngineSimulationBase {
 SimulationArray ParticleSims;ClothMatrix CurrentTransform,LastTransform,InverseTransform;const C3DMeshInfo* MeshInfo;unsigned int PrimitiveIndex;CEngineInternalPrimitiveAnimatedMesh* Owner;
 CEngineClothSimulationAnimatedMesh(const C3DMeshInfo*,unsigned int,CEngineInternalPrimitiveAnimatedMesh*); //00B81ED0
};
struct CEngineInternalPrimitiveAnimatedMesh_Methods:CEngineInternalPrimitiveAnimatedMesh {
 SimulationInfoPointer& Simulation(){return *reinterpret_cast<SimulationInfoPointer*>(_pad_0x64);}
 void InitialiseSimulation(const C3DMeshInfo*,const C3DMesh2&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveSimulationInfo)==20);
FABLE_STATIC_ASSERT(sizeof(CEngineClothSimulationAnimatedMesh)==180);
FABLE_STATIC_ASSERT(sizeof(C3DPrimitive2)==0xa0);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveSimulationInfo::ReleaseMethod)==4);
#endif
void CEngineInternalPrimitiveAnimatedMesh_Methods::InitialiseSimulation(const C3DMeshInfo* info,const C3DMesh2& mesh){
 if(!Simulation().Data){
  Simulation().Set(new CEnginePrimitiveSimulationInfo);
  for(unsigned int i=0;i<mesh.PrimitiveCount;++i){
   if(!mesh.Primitives[i].ClothPrimitives.Empty()){
    SimulationPointer simulation(new CEngineClothSimulationAnimatedMesh(info,i,this));Simulation().Data->AddSimulation(i,simulation);
   }
  }
  g_SimulationManager->AddSimulationInfo(Simulation(),false,false);
 }
}