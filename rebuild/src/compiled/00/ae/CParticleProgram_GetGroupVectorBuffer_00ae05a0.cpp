// CParticleProgram::GetGroupVectorBuffer(unsigned long count) -- reuse or replace the checked shared particle-program scratch buffer.
// Leaf: ?GetGroupVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z
// Identity: proven; both PDB exact private static fastcall CParticleProgram::GetGroupVectorBuffer(ulong) and own PrepareForRendering/ClingToMesh/TransformVertices caller roles; original element C4DVector stride16 and genuine CSimpleMemoryPool::AllocateType guard; FreeAll00A5C720/Allocate00A5C910 exact PDB calls; pool GroupVectorBufferPool original dynamic-initializer name, buffer/capacity globals descriptive at013D2DA8/013D2DAC, pool013D2DE8.
// Layout: shared pool/program data headers retained unchanged under aliases as evidence. Own RTTI restores CSimpleMemoryPool : CBaseClassNonCopyable : CBaseClass and CParticleProgram : CMovableResource. Pool nested CMemoryBlock/CArea names and original Size/NextFreeArea/Status fields restored; enum scope/enumerator are descriptive where no PDB key exists. Program only supplies static naming scope, no owner fields/sizeof used. Declared callable methods are original APIs. Original CMatrix4x4/C4DVector POD data have no shared headers. Checked allocator uses the actual64-bit product and32-bit byte limit, preserving failed allocation capacity behavior.
#define CSimpleMemoryPool CSimpleMemoryPool_HeaderFields
#include "engine/CSimpleMemoryPool.h"
#undef CSimpleMemoryPool
#define CParticleProgram CParticleProgram_HeaderFields
#include "engine/CParticleProgram.h"
#undef CParticleProgram
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long AllocationWide;
typedef char HostWideMustBe64[sizeof(AllocationWide)==8?1:-1];
#else
typedef unsigned __int64 AllocationWide;
#endif
class CMatrix4x4{public:float M[4][4];};class C4DVector{public:float X,Y,Z,W;};
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};class CMovableResource{public:virtual ~CMovableResource(){}virtual unsigned long Verify()const{return 0;}virtual bool CanRelocateData(long)const=0;};class CSimpleMemoryPool:public CBaseClassNonCopyable{public:class CMemoryBlock;enum EAreaStatus{AREA_STATUS_UNNAMED};class CArea{public:unsigned long Size;CArea* NextFreeArea;EAreaStatus Status;};CMemoryBlock* FirstMemoryBlock;unsigned long Capacity,GrowthGranularity;CArea Root;void FreeAll();void* Allocate(unsigned long);template<class T>T* AllocateType(unsigned long count){if(count){if((AllocationWide)count*sizeof(T)>0xFFFFFFFFUL)return 0;return (T*)Allocate(count*sizeof(T));}return 0;}};
extern C4DVector* g_GroupVectorBuffer_013D2DA8;extern unsigned long g_GroupVectorBufferCapacity_013D2DAC;extern CSimpleMemoryPool GroupVectorBufferPool;
class CParticleProgram:public CMovableResource{private:static C4DVector* __fastcall GetGroupVectorBuffer(unsigned long);friend struct TestAccess;};
C4DVector* __fastcall CParticleProgram::GetGroupVectorBuffer(unsigned long count){if(count>g_GroupVectorBufferCapacity_013D2DAC){GroupVectorBufferPool.FreeAll();C4DVector* buffer=GroupVectorBufferPool.AllocateType<C4DVector>(count);g_GroupVectorBufferCapacity_013D2DAC=count;g_GroupVectorBuffer_013D2DA8=buffer;}return g_GroupVectorBuffer_013D2DA8;}