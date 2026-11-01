// 00a65ff0 CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize
// Leaf: ??0CMemoryAllocatorFixedSize@@QAE@PAXKKKKK@Z
// Flags:
#include "rebuild_abi.h"
#define CMemoryAllocatorBase CMemoryAllocatorBaseFields
#include "engine/CMemoryAllocatorBase.h"
#undef CMemoryAllocatorBase
#define CMemoryAllocatorFixedSize CMemoryAllocatorFixedSizeFields
#define CMemoryAreaChunk CMemoryAreaChunkFields
#include "engine/CMemoryAllocatorFixedSize.h"
#undef CMemoryAreaChunk
#undef CMemoryAllocatorFixedSize
// Layout: true polymorphic base restored; unread statistics storage retains its original physical extent.
class CMemoryAllocatorBase{
public:
 class CPerformanceStats{public:unsigned long AllocsThisFrame,TotalAllocs,TotalFrames,MaxAllocsPerFrame;unsigned long TotalAllocsPerID[256],TotalAllocsPerIDLastFrame[256],TotalAllocsPerIDThisFrame[256];CPerformanceStats(){Clear();}void Clear(){AllocsThisFrame=TotalAllocs=TotalFrames=MaxAllocsPerFrame=0;for(unsigned long i=0;i<256;++i)TotalAllocsPerID[i]=0;}};
protected:CPerformanceStats PerformanceStats;unsigned long Alignment,AlignmentShift;
public:CMemoryAllocatorBase(unsigned long);
 virtual void*Allocate(unsigned long)=0;
 virtual void Deallocate(void*)=0;
 virtual unsigned long GetAllAllocatedAreas(void**,unsigned long)=0;
 virtual unsigned long GetAllocationSize(void*)=0;
 virtual unsigned long GetAllocationSizeIncludingHeader(void*)=0;
 virtual unsigned long GetLargestFreeAreaSize()=0;
 virtual unsigned long GetTotalMemoryControlledByAllocator()=0;
 virtual unsigned long GetTotalMemoryAllocated()=0;
 virtual unsigned long GetNoFreeAreas()=0;
 virtual unsigned long GetNoAllocatedAreas()=0;
 virtual float GetFragmentation()=0;
 virtual void*GetAllocatedArea(long)=0;
 virtual unsigned long GetHeaderOverhead()=0;
 virtual void ConsolidateFreeAreas();
 virtual void Validate()=0;
 unsigned long Align(unsigned long size)const{return((size+Alignment-1)>>AlignmentShift)<<AlignmentShift;}
};
class CMemoryAllocatorFixedSize:public CMemoryAllocatorBase{
protected:class CMemoryAreaChunk;
 CMemoryAreaChunk*PFirstChunk,*PLastChunk,*PCurrentAllocatorChunk;unsigned long AreaSize,StatAreaSize,ChunkHeaderSize,NoAreasPerChunk,ChunkSize,Flags,InternalFlags,ExtraMemUsedByOS;long NoEmptyChunks;
public:CMemoryAllocatorFixedSize(void*,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long);
 virtual unsigned long GetNoAllocatedAreas();virtual unsigned long GetNoFreeAreas();virtual void Validate();
 virtual~CMemoryAllocatorFixedSize();
 virtual void*Allocate(unsigned long);virtual void Deallocate(void*);virtual unsigned long GetAllocationSizeIncludingHeader(void*);virtual void*GetAllocatedArea(long);virtual unsigned long GetHeaderOverhead();virtual unsigned long GetLargestFreeAreaSize();virtual unsigned long GetAllocationSize(void*);virtual void ConsolidateFreeAreas();virtual unsigned long GetTotalMemoryControlledByAllocator();virtual unsigned long GetTotalMemoryAllocated();virtual unsigned long GetAllAllocatedAreas(void**,unsigned long);virtual float GetFragmentation();
};
CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize(void*,unsigned long chunkSize,unsigned long areaSize,unsigned long alignment,unsigned long flags,unsigned long internalFlags):CMemoryAllocatorBase(alignment),PCurrentAllocatorChunk(0),AreaSize((areaSize+4+alignment-1)/alignment*alignment-4),StatAreaSize(AreaSize+4),ChunkHeaderSize(Align(0x1c)-4),NoAreasPerChunk((chunkSize-ChunkHeaderSize)/StatAreaSize),ChunkSize(NoAreasPerChunk*StatAreaSize+ChunkHeaderSize),Flags(flags),InternalFlags(internalFlags),ExtraMemUsedByOS(0),NoEmptyChunks(0){PFirstChunk=0;PLastChunk=0;}
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorBase)==sizeof(CMemoryAllocatorBaseFields));FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorFixedSize)==sizeof(CMemoryAllocatorFixedSizeFields));