// CMemoryAllocatorFixedSize::Allocate(unsigned long) -- remove a free area and link it allocated.
// Native 0x00a66450, 250 bytes. PDB fixed-size CMemoryArea is four bytes (distinct from variable-size area).
// Actual allocator header is complete; extension adds methods without changing layout.
#include "engine/CMemoryAllocatorFixedSize.h"
struct CMemoryArea { unsigned char AreaID, NextAreaID, PreviousAreaID, AllocatorID; };
struct CMemoryAreaChunk {
 void* ThisChunkMem; long NoAllocatedAreas; CMemoryAreaChunk *PNextChunk,*PPreviousChunk;
 CMemoryArea *PFirstFreeArea,*PFirstAllocatedArea;
};
struct FixedSizeAllocator : CMemoryAllocatorFixedSize {
 CMemoryAreaChunk* NewChunk(); // 0x00a663d0
 void UnlinkChunk(CMemoryAreaChunk*); // 0x00a65e70
 void AppendChunk(CMemoryAreaChunk*); // 0x00a65f00, this retained from UnlinkChunk native callee
 void* Allocate(unsigned long);
};
#pragma auto_inline(off)
void FixedSizeAllocator::UnlinkChunk(CMemoryAreaChunk* c) {
 if(c->PNextChunk) c->PNextChunk->PPreviousChunk=c->PPreviousChunk;
 if(c->PPreviousChunk) c->PPreviousChunk->PNextChunk=c->PNextChunk;
 if(PFirstChunk==c) PFirstChunk=c->PNextChunk;
 if(PLastChunk==c) PLastChunk=c->PPreviousChunk;
}
void* FixedSizeAllocator::Allocate(unsigned long) {
 if (!PCurrentAllocatorChunk || NoAreasPerChunk-PCurrentAllocatorChunk->NoAllocatedAreas==0) {
  PCurrentAllocatorChunk=PFirstChunk;
  if (!PCurrentAllocatorChunk || NoAreasPerChunk-PCurrentAllocatorChunk->NoAllocatedAreas==0) {
   NewChunk(); PCurrentAllocatorChunk=PFirstChunk;
  }
 }
 CMemoryAreaChunk* c=PCurrentAllocatorChunk;
 CMemoryArea* a=c->PFirstFreeArea;
 long previous=c->NoAllocatedAreas;
 if (a) {
 c->PFirstFreeArea=a->NextAreaID!=255 ? (CMemoryArea*)((char*)a+(a->NextAreaID-a->AreaID)*StatAreaSize) : 0;
 if (c->PFirstAllocatedArea) {
  a->NextAreaID=c->PFirstAllocatedArea->AreaID;
  c->PFirstAllocatedArea->PreviousAreaID=a->AreaID;
 } else a->NextAreaID=255;
 c->PFirstAllocatedArea=a; ++c->NoAllocatedAreas; a->PreviousAreaID=255;
 if (!previous) --NoEmptyChunks;
 if (NoAreasPerChunk-PCurrentAllocatorChunk->NoAllocatedAreas==0) {
  UnlinkChunk(PCurrentAllocatorChunk); AppendChunk(PCurrentAllocatorChunk);
 }
 PCurrentAllocatorChunk=PFirstChunk;
 return a+1;
 }
 return 0;
}