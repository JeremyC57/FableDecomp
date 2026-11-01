// `dynamic atexit destructor for 'MatrixBufferPool'' -- at exit run the global pool's inline destructor: its vtable, ClearMemoryBlocks, then ~CBaseClassNonCopyable
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122AB10, 30 bytes (`mov ecx,offset MatrixBufferPool ; mov [ecx],0x0129A064 ; call 0x00A5C600 ; mov ecx,... ; jmp 0x0099A300`).
// Identity (naming wave 25, c692 queue: the ClearMemoryBlocks binders): the landed initialiser 0x01224450 ($E1 of the same TU) registers it with atexit; Ego_r.pdb / FableWin.pdb list `dynamic atexit destructor for 'MatrixBufferPool''. VC7.1 emits $E1 / $E2 from the one definition below (the initialiser row's source). Was a free ClearCachedDefs on an AutoTinyFieldMethodsTarget stand-in (First / Second_0099a300; the catalog label CTCInventoryItem::ClearCachedDefs is propagated).
// Identity: original CSimpleMemoryPool proven by own ctor00A5C570 and vtable0129A064/RTTI NonCopyable/CBaseClass bases; global013D2DB0 and callback0122AB10 are own producer/consumer addresses. Original global MatrixBufferPool is proven by both PDB initializer/atexit name sets and own N182 typed buffer accessor/caller chain; constructor(unsigned long,unsigned long) KK exact. Compiler emits actual initializer and atexit callback.
// Layout: actual CSimpleMemoryPool28, nested original class CArea12 fields Size/NextFreeArea/Status, CMemoryBlock pointer, Capacity/GrowthGranularity; protected NonCopyable ctor/dtor IAE/MAE from PDB namesets. Source headers retained. Donor Status enum display is EAreaStatus but its original qualified scope/enumerator names are unprovable; explicit CUnprovenAreaStatus projection preserves enum kind/4byte storage without asserting a false name. Only original ctor and real private ClearMemoryBlocks/dtor operations used; no fake vptr or callback function.
#define CSimpleMemoryPool CSimpleMemoryPool_HeaderEvidence
#include "engine/CSimpleMemoryPool.h"
#undef CSimpleMemoryPool
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};enum CUnprovenAreaStatus_00A5C570{CUnprovenAreaStatus_DescriptiveZero=0};class CSimpleMemoryPool:public CBaseClassNonCopyable{public:class CMemoryBlock;class CArea{public:unsigned long Size;CArea* NextFreeArea;CUnprovenAreaStatus_00A5C570 Status;};CMemoryBlock* FirstMemoryBlock;unsigned long Capacity,GrowthGranularity;CArea Root;CSimpleMemoryPool(unsigned long,unsigned long);virtual ~CSimpleMemoryPool(){ClearMemoryBlocks();}private:void ClearMemoryBlocks();};
CSimpleMemoryPool MatrixBufferPool(0,0x1000);
#ifdef _MSC_VER
typedef char PoolSize[(sizeof(CSimpleMemoryPool)==28)?1:-1];
#endif