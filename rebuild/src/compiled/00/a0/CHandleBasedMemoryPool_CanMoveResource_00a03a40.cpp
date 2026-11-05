// Leaf: ?CanMoveResource@CHandleBasedMemoryPool_Methods@@QAE_NHH@Z
#include "engine/CHandleBasedMemoryPool.h"  // retyped onto the PDB layout; byte parity re-verified
struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    bool CanMoveResource(int x0, int x1);
};
bool CHandleBasedMemoryPool_Methods::CanMoveResource(int x0, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CanMoveResource@CHandleBasedMemoryPool@@MAE_NPAXJ@Z); evidence: RTTI vtable slot + PDB name set.
