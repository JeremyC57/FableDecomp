// StaticInitialization::Initialize_MatrixBufferPool() -- construct the original static memory pool and register its destructor.
// Leaf: _$E1
// Identity: original CSimpleMemoryPool proven by own ctor00A5C570 and vtable0129A064/RTTI NonCopyable/CBaseClass bases; global013D2DB0 and callback0122AB10 are own producer/consumer addresses. Original global MatrixBufferPool is proven by both PDB initializer/atexit name sets and own N182 typed buffer accessor/caller chain; constructor(unsigned long,unsigned long) KK exact. Compiler emits actual initializer and atexit callback.
// Layout: actual CSimpleMemoryPool28, nested original class CArea12 fields Size/NextFreeArea/Status, CMemoryBlock pointer, Capacity/GrowthGranularity; protected NonCopyable ctor/dtor IAE/MAE from PDB namesets. Source headers retained. Donor Status enum display is EAreaStatus but its original qualified scope/enumerator names are unprovable; explicit CUnprovenAreaStatus projection preserves enum kind/4byte storage without asserting a false name. Only original ctor and real private ClearMemoryBlocks/dtor operations used; no fake vptr or callback function.
#define CSimpleMemoryPool CSimpleMemoryPool_HeaderEvidence
#include "engine/CSimpleMemoryPool.h"
#undef CSimpleMemoryPool
class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};enum CUnprovenAreaStatus_00A5C570{CUnprovenAreaStatus_DescriptiveZero=0};class CSimpleMemoryPool:public CBaseClassNonCopyable{public:class CMemoryBlock;class CArea{public:unsigned long Size;CArea* NextFreeArea;CUnprovenAreaStatus_00A5C570 Status;};CMemoryBlock* FirstMemoryBlock;unsigned long Capacity,GrowthGranularity;CArea Root;CSimpleMemoryPool(unsigned long,unsigned long);virtual ~CSimpleMemoryPool(){ClearMemoryBlocks();}private:void ClearMemoryBlocks();};
CSimpleMemoryPool MatrixBufferPool(0,0x1000);
#ifdef _MSC_VER
typedef char PoolSize[(sizeof(CSimpleMemoryPool)==28)?1:-1];
#endif