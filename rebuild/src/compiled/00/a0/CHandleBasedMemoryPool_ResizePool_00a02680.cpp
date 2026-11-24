// CHandleBasedMemoryPool::ResizePool(unsigned long, float)
// Leaf: ?ResizePool@CHandleBasedMemoryPool@@QAE_NKM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02680, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// engine/CHandleBasedMemoryPool.h (PDB): +0x0C PoolSize. The pool is quiesced with the second
// argument (BeginShutdown(float) @ 0x00A02220 returns false when it cannot be), the new size is
// recorded, and the pool is brought back (EndShutdown @ 0x00A02370). Callee names are the
// manifest's; the float is the time budget BeginShutdown is allowed.
//
// Lane feedback: `if (ok) { ...; return true; } return false;` gives retail's block order; the early
// `if (!ok) return false;` form differs by 2 bytes. RELOCATION_MATCH.
// Name (NF378): Ego_r.pdb / FableWin.pdb ?ResizePool@CHandleBasedMemoryPool@@QAE_NKM@Z; BSim 1.0 and Ego_r
// lib_handle_based_memory_pool.obj order (was the stand-in CHandleBasedMemoryPool_Methods; its binders 0x0041D122 / 0x0041D165 move with it).
#define CHandleBasedMemoryPool CHandleBasedMemoryPoolLayout
#include "engine/CHandleBasedMemoryPool.h"
#undef CHandleBasedMemoryPool

class CHandleBasedMemoryPool : public CHandleBasedMemoryPoolLayout
{
public:
    bool BeginShutdown(float);   // 0x00A02220
    void EndShutdown();   // 0x00A02370
    bool ResizePool(unsigned long newPoolSize, float timeBudget);
};

bool CHandleBasedMemoryPool::ResizePool(unsigned long newPoolSize, float timeBudget)
{
    // Only when the pool can be shut down within the budget: adopt the new size and bring the
    // pool back up. The success path is the fall-through; the failure exit is the cold tail.
    if (BeginShutdown(timeBudget)) {
        this->PoolSize = newPoolSize;
        EndShutdown();
        return true;
    }
    return false;
}