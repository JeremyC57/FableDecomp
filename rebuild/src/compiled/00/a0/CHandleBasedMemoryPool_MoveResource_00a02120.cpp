// Leaf: ?MoveResource@CHandleBasedMemoryPool@@MAEXPAXJ00K@Z
// CHandleBasedMemoryPool_Methods::MoveResource forwarder
// Retail forwards to a cdecl helper, ignoring this/p0/p1.
// Header route (NF319, 2026-11-23): defined on the real class of engine/CHandleBasedMemoryPool.h (was ?MoveResource@CHandleBasedMemoryPool_Methods@@QAEXPAXJ00K@Z); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address;
//   declared there under FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL (switches landed rows define). Bytes unchanged.
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL

#include "engine/CHandleBasedMemoryPool.h"  // retyped onto the PDB layout; byte parity re-verified
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (MSVCR71)


void CHandleBasedMemoryPool::MoveResource(void* p0, long p1, void* p2, void* p3, unsigned long p4) {
    memmove(p3, p2, p4);
}