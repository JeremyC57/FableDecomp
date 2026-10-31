// 00a659c0 CMemoryAllocatorVariableSize::Allocate
// Codex parked-50 best attempt: DIFFER(140v140); residue: 140/140 bytes; the Alignment and AlignmentShift reloads occur in the opposite order.
// Leaf: ?Allocate@CMemoryAllocatorVariableSize@@UAEPAXK@Z
// Flags:
#include "rebuild_abi.h"
#define CMemoryAllocatorBase CMemoryAllocatorBaseFields
#include "engine/CMemoryAllocatorBase.h"
#undef CMemoryAllocatorBase
// Layout: the unread performance-statistics object is retained physically; native alignment fields and true polymorphic base identity are restored.
class CMemoryAllocatorBase{public:virtual void*Allocate(unsigned long)=0;protected:unsigned char PerformanceStats[0xc10];unsigned long Alignment,AlignmentShift;static unsigned long CurrentAlignment,CurrentAlignmentShift;public:
 void SetCurrentAlignment(){CurrentAlignmentShift=AlignmentShift;CurrentAlignment=Alignment;}
 unsigned long Align(unsigned long size)const{const unsigned long alignment=Alignment;size+=alignment;--size;size>>=AlignmentShift;size<<=AlignmentShift;return size;}
 static unsigned long __fastcall StaticAlign(unsigned long size){return((size+CurrentAlignment-1)>>CurrentAlignmentShift)<<CurrentAlignmentShift;}
};
class CMemoryAllocatorVariableSize:public CMemoryAllocatorBase{public:class CMemoryArea;class CMemoryAreaChunk{unsigned char unread[52];public:CMemoryArea*AllocAreaFromArea(CMemoryArea*,long);};
 class CMemoryArea{public:CMemoryArea*PNextArea,*PRightChild,*PPreviousArea,*PParentArea,*PLeftChild;CMemoryAreaChunk*PParentChunk;long DataSize;unsigned char Colour,ID;static long __fastcall GetHeaderSize(){return CMemoryAllocatorBase::StaticAlign(sizeof(CMemoryArea));}void*GetDataStart()const{return(char*)this+GetHeaderSize();}};
 CMemoryAreaChunk*PFirstChunk;long MinChunkSize,NoFreesSinceConsolidation;unsigned long Flags,InternalFlags,ExtraMemUsedByOS;CMemoryArea*PTreeRoot;
 virtual~CMemoryAllocatorVariableSize();virtual void*Allocate(unsigned long);
 CMemoryArea*AllocateMemoryFromExistingChunks(unsigned long size){size=Align(size);CMemoryArea*area=GetTreeAreaWithSizeGreaterThanOrEqualTo(size);if(area)return area->PParentChunk->AllocAreaFromArea(area,size);return 0;}
protected:CMemoryArea*GetTreeAreaWithSizeGreaterThanOrEqualTo(long)const;CMemoryAreaChunk*NewChunk(long);
};
void*CMemoryAllocatorVariableSize::Allocate(unsigned long size){SetCurrentAlignment();CMemoryArea*area=AllocateMemoryFromExistingChunks(size);if(!area){if(!NewChunk(size))return 0;area=AllocateMemoryFromExistingChunks(size);}if(area)return area->GetDataStart();return 0;}
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorBase)==sizeof(CMemoryAllocatorBaseFields));FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorVariableSize)==3128);FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorVariableSize::CMemoryArea)==32);
