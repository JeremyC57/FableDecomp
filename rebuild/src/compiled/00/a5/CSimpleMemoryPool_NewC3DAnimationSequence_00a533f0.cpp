// 00a533f0 CSimpleMemoryPool::New<C3DAnimationSequence>
// Leaf: ??$New@VC3DAnimationSequence@@@CSimpleMemoryPool@@QAEPAVC3DAnimationSequence@@K@Z
// Flags:
#include <new>
#define CSimpleMemoryPool CSimpleMemoryPoolLayout
#include "engine/CSimpleMemoryPool.h"
#undef CSimpleMemoryPool
class C3DAnimationSequenceData;
// Layout: original PDB class8 bytes, private Data0, ParentIndex4, Shared6; native ctor stores agree.
class C3DAnimationSequence {private:C3DAnimationSequenceData*Data;short ParentIndex;bool Shared;public:C3DAnimationSequence():Data(0),ParentIndex(-1),Shared(false){}};
// Layout: the supplied pool header flattens its vptr; no field or pointer adjustment added here.
class CSimpleMemoryPool:public CSimpleMemoryPoolLayout {
public:void*Allocate(unsigned long);
 template<class T>T*New(unsigned long count){
  T*data;
  if(count){unsigned __int64 bytes=(unsigned __int64)count*sizeof(T);if((unsigned long)(bytes>>32)<=0UL&&(unsigned long)bytes<=0xffffffffUL)data=static_cast<T*>(Allocate(count*sizeof(T)));else data=0;}else data=0;
  unsigned long remaining=count;if(remaining>0){T*cursor=data;for(;remaining>0;--remaining,++cursor)if(cursor)new(cursor)T;}
  return data;
 }
};
template C3DAnimationSequence*CSimpleMemoryPool::New<C3DAnimationSequence>(unsigned long);