// Exact selected function and its genuine helper bodies; external chunk services are controlled.
#include <cstdio>
#include <cassert>
#include <cstring>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
#define FABLE_STATIC_ASSERT(x)
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
 // Retail opt-in (Codex pass 17): original SetCurrentAlignment body, both PDBs.
 void SetCurrentAlignment(){CurrentAlignmentShift=AlignmentShift;CurrentAlignment=Alignment;}
#endif
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
        // Retail opt-in (Codex pass 17): public original nested-class method.
        CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size);
#endif
        void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);   // 0x00A65410
    };
    class CMemoryArea {                         // Ego_r.pdb: 32 bytes
    public:
        static long FABLE_FASTCALL GetHeaderSize(){return CMemoryAllocatorBase::StaticAlign(sizeof(CMemoryArea));}
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
        void* GetDataStart() const {return (char*)this + GetHeaderSize();}
#endif
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
    virtual void* Allocate(unsigned long size);
    CMemoryArea* AllocateMemoryFromExistingChunks(unsigned long size)
    {
        unsigned long data_size = Align(size);
        CMemoryArea* pbest_area = GetTreeAreaWithSizeGreaterThanOrEqualTo(data_size);
        if (pbest_area) {
            CMemoryAreaChunk* pchunk = pbest_area->PParentChunk;
            return pchunk->AllocAreaFromArea(pbest_area, data_size);
        }
        return 0;
    }
protected:
    CMemoryArea* GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const;
public:
#endif
    long InternalAlloc(long no_bytes,void** pdata);
protected:
    CMemoryAreaChunk* NewChunk(long request_size);
};

#endif
// Original best-fit tree walk is visible to this translation unit.
CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const
{
    CMemoryArea* best = 0;
    CMemoryArea* area = PTreeRoot;
    while (area) {
        if (area->DataSize == size) return area;
        if (area->DataSize > size) {
            if (!best || area->DataSize < best->DataSize) best = area;
            area = area->PLeftChild;
        } else {
            area = area->PRightChild;
        }
    }
    return best;
}
void* CMemoryAllocatorVariableSize::Allocate(unsigned long size)
{
    SetCurrentAlignment();
    CMemoryArea* parea = AllocateMemoryFromExistingChunks(size);
    if (!parea) {
        if (NewChunk(size))
            parea = AllocateMemoryFromExistingChunks(size);
    }
    if (parea)
        return parea->GetDataStart();
    return 0;
}

unsigned long CMemoryAllocatorBase::CurrentAlignment=0;
unsigned long CMemoryAllocatorBase::CurrentAlignmentShift=0;
void CMemoryAllocatorBase::ConsolidateFreeAreas(){}
static int new_calls,carve_calls,fail_carves;
static bool grow_succeeds;
static unsigned long expected_size;
static CMemoryAllocatorVariableSize::CMemoryAreaChunk test_chunk;
static CMemoryAllocatorVariableSize::CMemoryArea grown;
CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area,long size)
{
    assert(this==&test_chunk && area->PParentChunk==this);
    assert((unsigned long)size==expected_size);
    ++carve_calls;
    if(carve_calls<=fail_carves) return 0;
    return area;
}
CMemoryAllocatorVariableSize::CMemoryAreaChunk*
CMemoryAllocatorVariableSize::NewChunk(long size)
{
    ++new_calls;assert(Align(size)==expected_size);
    if(!grow_succeeds)return 0;
    std::memset(&grown,0,sizeof(grown));grown.DataSize=65536;grown.PParentChunk=&test_chunk;PTreeRoot=&grown;
    return &test_chunk;
}
class TestAllocator:public CMemoryAllocatorVariableSize {
public:
 void Configure(unsigned long alignment,unsigned long shift,CMemoryArea* root){Alignment=alignment;AlignmentShift=shift;PTreeRoot=root;}
 CMemoryArea* Search(long n){return GetTreeAreaWithSizeGreaterThanOrEqualTo(n);}
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
int main()
{
 const unsigned long requests[]={0,1,3,4,7,16,17,99,255};
 int cases=0;
 for(unsigned long shift=2;shift<=4;++shift)for(int r=0;r<9;++r)for(int mode=0;mode<5;++mode){
    unsigned long alignment=1UL<<shift;
    expected_size=((requests[r]+alignment-1)/alignment)*alignment;
    TestAllocator allocator;
    CMemoryAllocatorVariableSize::CMemoryArea area;
    std::memset(&area,0,sizeof(area));area.DataSize=65536;area.PParentChunk=&test_chunk;
    allocator.Configure(alignment,shift,(mode==1||mode==2)?0:&area);
    new_calls=carve_calls=0;grow_succeeds=(mode!=2);fail_carves=mode==3?1:mode==4?2:0;
    void* result=allocator.Allocate(requests[r]);
    CMemoryAllocatorVariableSize::CMemoryArea* expected=mode==0?&area:mode==1||mode==3?&grown:0;
    unsigned long header=((sizeof(area)+alignment-1)/alignment)*alignment;
    assert(result==(expected?(char*)expected+header:0));
    assert(new_calls==(mode==0?0:1));
    assert(carve_calls==(mode==2?0:(mode>=3?2:1)));
    ++cases;
 }
 // Verify actual best-fit helper, including exact matches and no suitable area.
 CMemoryAllocatorVariableSize::CMemoryArea nodes[7];std::memset(nodes,0,sizeof(nodes));
 for(int i=0;i<7;++i)nodes[i].DataSize=(i+1)*16;
 nodes[3].PLeftChild=&nodes[1];nodes[3].PRightChild=&nodes[5];
 nodes[1].PLeftChild=&nodes[0];nodes[1].PRightChild=&nodes[2];
 nodes[5].PLeftChild=&nodes[4];nodes[5].PRightChild=&nodes[6];
 TestAllocator allocator;allocator.Configure(4,2,&nodes[3]);
 for(long n=0;n<=128;++n){int index=(int)((n+15)/16)-1;if(index<0)index=0;assert(allocator.Search(n)==(index<7?&nodes[index]:0));++cases;}
 assert(cases==264);std::puts("00a659c0_TEST PASS");
}