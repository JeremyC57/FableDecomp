// CMemoryAllocatorFixedSize::Deallocate(void*) -- return an area to its chunk free list.
// Native 0x00a662a0, 242 bytes. PDB fixed-size CMemoryArea is four bytes (distinct from variable-size area).
// Actual allocator header is complete; extension adds methods without changing layout.
#include "engine/CMemoryAllocatorFixedSize.h"
struct CMemoryArea { unsigned char AreaID, NextAreaID, PreviousAreaID, AllocatorID;
 __forceinline CMemoryArea* GetNext(unsigned long size) { return NextAreaID!=255 ? (CMemoryArea*)((char*)this+(NextAreaID-AreaID)*size) : 0; }
 __forceinline CMemoryArea* GetPrevious(unsigned long size) { return PreviousAreaID!=255 ? (CMemoryArea*)((char*)this+(PreviousAreaID-AreaID)*size) : 0; }
 __forceinline void SetNext(unsigned char id) { NextAreaID=id; }
 __forceinline void SetPrevious(unsigned char id) { PreviousAreaID=id; }
};
struct CMemoryAreaChunk {
 void* ThisChunkMem; long NoAllocatedAreas; CMemoryAreaChunk *PNextChunk,*PPreviousChunk;
 CMemoryArea *PFirstFreeArea,*PFirstAllocatedArea;
};
struct FixedSizeAllocator : CMemoryAllocatorFixedSize {
 CMemoryAreaChunk* NewChunk(); // 0x00a663d0
 void UnlinkChunk(CMemoryAreaChunk*); // 0x00a65e70
 void AppendChunk(CMemoryAreaChunk*); // 0x00a65f00, this retained from UnlinkChunk native callee
 void Deallocate(void*);
 void PrependChunk(CMemoryAreaChunk*); // 0x00a65ec0
 void DeleteEmptyChunk(CMemoryAreaChunk*); // 0x00a66160
};
#pragma auto_inline(off)
void FixedSizeAllocator::UnlinkChunk(CMemoryAreaChunk* c) {
 if(c->PNextChunk) c->PNextChunk->PPreviousChunk=c->PPreviousChunk;
 if(c->PPreviousChunk) c->PPreviousChunk->PNextChunk=c->PNextChunk;
 if(PFirstChunk==c) PFirstChunk=c->PNextChunk;
 if(PLastChunk==c) PLastChunk=c->PPreviousChunk;
}
void FixedSizeAllocator::PrependChunk(CMemoryAreaChunk* c) {
 c->PNextChunk=PFirstChunk; if(PFirstChunk) PFirstChunk->PPreviousChunk=c;
 c->PPreviousChunk=0; CMemoryAreaChunk* oldFirst=PFirstChunk; PFirstChunk=c; if(!oldFirst) PLastChunk=c;
}
void FixedSizeAllocator::Deallocate(void* p) {
 CMemoryArea* a=(CMemoryArea*)p-1;
 CMemoryAreaChunk* c=!a ? 0 : (CMemoryAreaChunk*)((char*)a-a->AreaID*StatAreaSize-ChunkHeaderSize);
 if(a->PreviousAreaID!=255) {
  a->GetPrevious(StatAreaSize)->SetNext(a->NextAreaID);
 } else c->PFirstAllocatedArea=a->NextAreaID!=255 ? (CMemoryArea*)((char*)a+(a->NextAreaID-a->AreaID)*StatAreaSize) : 0;
 if(a->NextAreaID!=255) {
  a->GetNext(StatAreaSize)->SetPrevious(a->PreviousAreaID);
 }
 if(c->PFirstFreeArea) {
  c->PFirstFreeArea->PreviousAreaID=a->AreaID;
  a->NextAreaID=c->PFirstFreeArea->AreaID;
 } else a->NextAreaID=255;
 c->PFirstFreeArea=a; --c->NoAllocatedAreas;
 if(NoAreasPerChunk-c->NoAllocatedAreas==1 && PFirstChunk!=c) { UnlinkChunk(c); PrependChunk(c); }
 if(c->NoAllocatedAreas==0 && ++NoEmptyChunks>1) DeleteEmptyChunk(c);
}