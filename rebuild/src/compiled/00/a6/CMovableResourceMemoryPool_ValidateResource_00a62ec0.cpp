// Leaf: ?ValidateResource@CMovableResourceMemoryPool@@MAEXPAX@Z
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CMovableResourceMemoryPool.h (class-model switch FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL; was ?ValidateResource@CMovableResourceMemoryPool_Methods@@QAEXPAX@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CMovableResourceMemoryPool_CMovableResourceMemoryPool_00a62e20.cpp.
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#include "engine/CMovableResourceMemoryPool.h"
struct IValidatable
{
    void* vtbl[4];
};

typedef void (__fastcall *ValidateFn)(void* self);

void CMovableResourceMemoryPool::ValidateResource(void* p)
{
    ValidateFn fn = (ValidateFn)(*(void***)p)[3];
    fn(p);
}