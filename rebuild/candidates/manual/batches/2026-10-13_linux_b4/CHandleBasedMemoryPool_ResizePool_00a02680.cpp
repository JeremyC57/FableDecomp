// CHandleBasedMemoryPool::ResizePool(unsigned long, float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02680, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+8] ; push esi ; push eax ; mov esi,ecx ; call CHandleBasedMemoryPool::BeginShutdown ; test al,al ; je F ; mov ecx,[esp+8] ; mov [esi+0Ch],ecx ; mov ecx,esi ; call CHandleBasedMemoryPool::EndShutdown ; mov al,1 ; pop esi ; ret 8 ; F: xor al,al ; pop esi ; ret 8
// engine/CHandleBasedMemoryPool.h (PDB): +0x0C PoolSize. The pool is quiesced with the second
// argument (BeginShutdown(float) @ 0x00A02220 returns false when it cannot be), the new size is
// recorded, and the pool is brought back (EndShutdown @ 0x00A02370). Callee names are the
// manifest's; the float is the time budget BeginShutdown is allowed.
//
// Lane feedback: `if (ok) { ...; return true; } return false;` gives retail's block order; the early
// `if (!ok) return false;` form differs by 2 bytes. RELOCATION_MATCH.
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    bool BeginShutdown(float timeBudget);     // 0x00A02220
    void EndShutdown();                       // 0x00A02370
    bool ResizePool(unsigned long newPoolSize, float timeBudget);
};

bool CHandleBasedMemoryPool_Methods::ResizePool(unsigned long newPoolSize, float timeBudget)
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
