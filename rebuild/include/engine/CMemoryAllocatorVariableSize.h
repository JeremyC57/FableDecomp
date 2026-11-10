#ifndef FABLE_ENGINE_CMEMORYALLOCATORVARIABLESIZE_H
#define FABLE_ENGINE_CMEMORYALLOCATORVARIABLESIZE_H
#include "engine/CMemoryAllocatorBase.h"
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOC_FROM_AREA
    protected:
        void RemoveAreaFromFreeList(CMemoryArea*);
        void InsertAreaIntoFreeList(CMemoryArea*);
        void InsertAreaIntoAllocatedList(CMemoryArea* parea);
    public:
#endif
        void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);   // 0x00A65410
    };
    class CMemoryArea {                         // Ego_r.pdb: 32 bytes
    public:
        static long FABLE_FASTCALL GetHeaderSize(){return CMemoryAllocatorBase::StaticAlign(sizeof(CMemoryArea));}
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
        void* GetDataStart() const {return (char*)this + GetHeaderSize();}
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOC_FROM_AREA
        static CMemoryArea* FABLE_FASTCALL Create(void* pstart,CMemoryAreaChunk* pparent_chunk,unsigned long data_size);
        long GetDataSize()const{return DataSize;}
        long GetTotalSize()const{return DataSize+GetHeaderSize();}
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

#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOC_FROM_AREA
inline CMemoryAllocatorVariableSize::CMemoryArea* FABLE_FASTCALL CMemoryAllocatorVariableSize::CMemoryArea::Create(void* pstart,CMemoryAreaChunk* pparent_chunk,unsigned long data_size)
{
 CMemoryArea* parea=(CMemoryArea*)pstart;
 parea->PNextArea=0;parea->PRightChild=0;parea->PPreviousArea=0;parea->PParentArea=0;parea->PLeftChild=0;
 parea->Colour=0;parea->PParentChunk=pparent_chunk;parea->DataSize=data_size;
 return parea;
}
inline void CMemoryAllocatorVariableSize::CMemoryAreaChunk::InsertAreaIntoAllocatedList(CMemoryArea* parea)
{
 parea->PNextArea=PFirstAllocatedArea;
 parea->PPreviousArea=0;
 if(PFirstAllocatedArea)PFirstAllocatedArea->PPreviousArea=parea;
 PFirstAllocatedArea=parea;
 ++NoAllocatedAreas;
}
#endif

#endif
