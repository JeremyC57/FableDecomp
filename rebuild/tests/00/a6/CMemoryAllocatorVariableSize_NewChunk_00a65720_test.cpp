// Test boundary injection: production body below is unchanged.
#define VirtualAlloc TestVirtualAlloc
#define VirtualQuery TestVirtualQuery
#include <stdio.h>
#include <assert.h>
#include <string.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#define FABLE_STDCALL __stdcall
#else
#define FABLE_FASTCALL
#define FABLE_STDCALL
#endif
#define FABLE_STATIC_ASSERT(x)
// Retail opt-in (Codex 2026-11-10): original PDB virtual interface,
// protected alignment fields and static alignment state. Native size0xc1c.
inline unsigned long FABLE_FASTCALL GFAlignShift(unsigned long value,unsigned long alignment,unsigned long shift)
{return ((value+alignment-1)>>shift)<<shift;}
class CMemoryAllocatorBase {
protected:
 unsigned char PerformanceStats[0xc10];
 unsigned long Alignment,AlignmentShift;
 static unsigned long CurrentAlignment,CurrentAlignmentShift;
public:
 virtual void* Allocate(unsigned long)=0;
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
 virtual void* GetAllocatedArea(long)=0;
 virtual unsigned long GetHeaderOverhead()=0;
 virtual void ConsolidateFreeAreas();
 virtual void Validate()=0;
 static unsigned long FABLE_FASTCALL StaticAlign(unsigned long value){return GFAlignShift(value,CurrentAlignment,CurrentAlignmentShift);}
 unsigned long Align(unsigned long value)const{return GFAlignShift(value,Alignment,AlignmentShift);}
};
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorBase)==0xc1c);
#ifndef FABLE_ENGINE_CMEMORYALLOCATORVARIABLESIZE_H
#define FABLE_ENGINE_CMEMORYALLOCATORVARIABLESIZE_H
// Original PDB nested types and complete storage; native field offsets preserved.
class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase {   // Ego_r.pdb: 0xc38 bytes
public:
    class CMemoryArea;
    class CMemoryAreaChunk {                    // Ego_r.pdb: 52 bytes
    public:
        static unsigned long FABLE_FASTCALL GetHeaderSize(){return CMemoryAllocatorBase::StaticAlign(sizeof(CMemoryAreaChunk));}
        long              TotalDataSize;        // +0x00
        long              NoFreeAreas;          // +0x04
        long              NoAllocatedAreas;     // +0x08
        long              FreeDataSize;         // +0x0c
        long              LargestFreeAreaSize;  // +0x10
        long              SmallestFreeAreaSize; // +0x14
        CMemoryAllocatorVariableSize* PParentAllocator;   // +0x18
        bool              Permanent;            // +0x1c
        bool              DemandPaged;          // +0x1d
        CMemoryAreaChunk* PNextChunk;           // +0x20
        CMemoryAreaChunk* PPreviousChunk;       // +0x24
        CMemoryArea*      PFirstFreeArea;       // +0x28
        CMemoryArea*      PLastFreeArea;        // +0x2c
        CMemoryArea*      PFirstAllocatedArea;  // +0x30
        void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);   // 0x00A65410
    };
    class CMemoryArea {                         // Ego_r.pdb: 32 bytes
    public:
        static long FABLE_FASTCALL GetHeaderSize(){return CMemoryAllocatorBase::StaticAlign(sizeof(CMemoryArea));}
        CMemoryArea*      PNextArea;            // +0x00
        CMemoryArea*      PRightChild;          // +0x04
        CMemoryArea*      PPreviousArea;        // +0x08
        CMemoryArea*      PParentArea;          // +0x0c
        CMemoryArea*      PLeftChild;           // +0x10
        CMemoryAreaChunk* PParentChunk;         // +0x14
        long              DataSize;             // +0x18
        unsigned char     Colour;               // +0x1c
        unsigned char     ID;                   // +0x1d
    };
protected:
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;   // +0xc24
    unsigned long     Flags;                    // +0xc28 (bit 1: read-only pages)
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
public:
    long InternalAlloc(long no_bytes,void** pdata);
protected:
    CMemoryAreaChunk* NewChunk(long request_size);
};

#endif

class CMemoryAllocator {
protected: static CMemoryAllocator* PMemoryAllocator;
public:
 static CMemoryAllocator* FABLE_FASTCALL Get(){return PMemoryAllocator;}
 void OnFailedAllocation();
};
struct _MEMORY_BASIC_INFORMATION {
 void* BaseAddress;void* AllocationBase;unsigned long AllocationProtect;
 unsigned long RegionSize,State,Protect,Type;
};
extern "C" void* FABLE_STDCALL VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned long FABLE_STDCALL VirtualQuery(const void*,_MEMORY_BASIC_INFORMATION*,unsigned long);
inline long CMemoryAllocatorVariableSize::InternalAlloc(long no_bytes,void** pdata)
{
 unsigned long page_flags=0;
 if(Flags&2)page_flags|=2;else page_flags|=4;
 long size_allocated=0;
 *pdata=VirtualAlloc(0,no_bytes,0x103000,page_flags);
 if(*pdata){
  _MEMORY_BASIC_INFORMATION info;
  VirtualQuery(*pdata,&info,sizeof(info));
  size_allocated=info.RegionSize;
 }
 return size_allocated;
}
CMemoryAllocatorVariableSize::CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long request_size)
{
 long real_size=Align(CMemoryAreaChunk::GetHeaderSize()+request_size+CMemoryArea::GetHeaderSize()>(unsigned long)MinChunkSize?
  CMemoryAreaChunk::GetHeaderSize()+request_size+CMemoryArea::GetHeaderSize():MinChunkSize);
 void* pdata=0;
 long allocated_size=0;
 long try_size=real_size;
 while(pdata==0 && try_size>=request_size){
  allocated_size=InternalAlloc(try_size,&pdata);
  try_size/=2;
 }
 if(!pdata){CMemoryAllocator::Get()->OnFailedAllocation();return 0;}
 long data_size=allocated_size-CMemoryAreaChunk::GetHeaderSize();
 CMemoryAreaChunk* pchunk=(CMemoryAreaChunk*)pdata;
 pchunk->Init((char*)pdata+CMemoryAreaChunk::GetHeaderSize(),data_size,this);
 pchunk->PNextChunk=PFirstChunk;
 if(PFirstChunk)PFirstChunk->PPreviousChunk=pchunk;
 pchunk->PPreviousChunk=0;
 PFirstChunk=pchunk;
 return pchunk;
}

static CMemoryAllocator memory_manager;
CMemoryAllocator* CMemoryAllocator::PMemoryAllocator=&memory_manager;
unsigned long CMemoryAllocatorBase::CurrentAlignment=16;
unsigned long CMemoryAllocatorBase::CurrentAlignmentShift=4;
void CMemoryAllocatorBase::ConsolidateFreeAreas() {}
static int failed,call_count,succeed_at,queries,init_count,block_index;
static unsigned long expected_protection,requested[32],last_size;
union TestBlock { double align; void* pointer_align; unsigned char bytes[8192]; } blocks[2];
void CMemoryAllocator::OnFailedAllocation(){++failed;}
extern "C" void* FABLE_STDCALL VirtualAlloc(void* address,unsigned long size,unsigned long type,unsigned long protection)
{
 assert(address==0 && type==0x103000 && protection==expected_protection);
 assert(call_count<32);requested[call_count++]=size;last_size=size;
 if(call_count<succeed_at)return 0;
 assert(size<=8192);return blocks[block_index].bytes;
}
extern "C" unsigned long FABLE_STDCALL VirtualQuery(const void* address,_MEMORY_BASIC_INFORMATION* info,unsigned long size)
{
 assert(address==blocks[block_index].bytes && size==sizeof(*info));
 ++queries;memset(info,0,sizeof(*info));info->RegionSize=last_size;return size;
}
void CMemoryAllocatorVariableSize::CMemoryAreaChunk::Init(void* memory,long size,CMemoryAllocatorVariableSize* parent)
{
 ++init_count;assert(memory==(char*)this+GetHeaderSize());
 assert(size==(long)last_size-(long)GetHeaderSize());PParentAllocator=parent;TotalDataSize=size;
}
class TestAllocator:public CMemoryAllocatorVariableSize {
public:
 void Configure(long minimum,unsigned long flags){MinChunkSize=minimum;Flags=flags;Alignment=16;AlignmentShift=4;PFirstChunk=0;}
 CMemoryAreaChunk* Run(long n){return NewChunk(n);}
 CMemoryAreaChunk* Head()const{return PFirstChunk;}
 virtual void* Allocate(unsigned long){return 0;}
 virtual void Deallocate(void*){}
 virtual unsigned long GetAllAllocatedAreas(void**,unsigned long){return 0;}
 virtual unsigned long GetAllocationSize(void*){return 0;}
 virtual unsigned long GetAllocationSizeIncludingHeader(void*){return 0;}
 virtual unsigned long GetLargestFreeAreaSize(){return 0;}
 virtual unsigned long GetTotalMemoryControlledByAllocator(){return 0;}
 virtual unsigned long GetTotalMemoryAllocated(){return 0;}
 virtual unsigned long GetNoFreeAreas(){return 0;}
 virtual unsigned long GetNoAllocatedAreas(){return 0;}
 virtual float GetFragmentation(){return 0;}
 virtual void* GetAllocatedArea(long){return 0;}
 virtual unsigned long GetHeaderOverhead(){return 0;}
 virtual void Validate(){}
};
static unsigned long Round16(unsigned long n){return ((n+15)/16)*16;}
int main()
{
 const long requests[4]={16,40,90,400};
 for(int r=0;r<4;++r)for(int mode=0;mode<2;++mode)for(int failmode=0;failmode<3;++failmode)
 {
  TestAllocator allocator;allocator.Configure(1024,mode?2:0);
  expected_protection=mode?2:4;failed=call_count=queries=init_count=0;block_index=0;
  succeed_at=failmode==0?1:failmode==1?2:100;
  memset(blocks,0,sizeof blocks);
  CMemoryAllocatorVariableSize::CMemoryAreaChunk* chunk=allocator.Run(requests[r]);
  unsigned long total=Round16(sizeof(CMemoryAllocatorVariableSize::CMemoryAreaChunk))+requests[r]+Round16(sizeof(CMemoryAllocatorVariableSize::CMemoryArea));
  if(total<1024)total=1024;total=Round16(total);
  assert(requested[0]==total);
  for(int k=1;k<call_count;++k)assert(requested[k]==requested[k-1]/2);
  if(failmode==2){assert(chunk==0 && failed==1 && queries==0 && init_count==0 && allocator.Head()==0);}
  else{
   assert(chunk==(void*)blocks[0].bytes && chunk->PPreviousChunk==0 && chunk->PNextChunk==0);
   assert(chunk->PParentAllocator==&allocator && failed==0 && queries==1 && init_count==1);
   assert(allocator.Head()==chunk);
   block_index=1;call_count=0;succeed_at=1;
   CMemoryAllocatorVariableSize::CMemoryAreaChunk* next=allocator.Run(requests[r]);
   assert(next==(void*)blocks[1].bytes && next->PNextChunk==chunk && chunk->PPreviousChunk==next);
   assert(next->PPreviousChunk==0 && allocator.Head()==next);
  }
 }
 puts("00a65720_TEST PASS");return 0;
}