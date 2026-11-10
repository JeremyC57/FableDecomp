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
