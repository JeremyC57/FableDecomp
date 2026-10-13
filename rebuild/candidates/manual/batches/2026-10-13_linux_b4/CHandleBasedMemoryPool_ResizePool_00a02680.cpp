// CHandleBasedMemoryPool::ResizePool(unsigned long, float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02680, oracle length 43 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+8] ; push esi ; push eax ; mov esi,ecx ; call CHandleBasedMemoryPool::BeginShutdown ; test al,al ; je F ; mov ecx,[esp+8] ; mov [esi+0Ch],ecx ; mov ecx,esi ; call CHandleBasedMemoryPool::EndShutdown ; mov al,1 ; pop esi ; ret 8 ; F: xor al,al ; pop esi ; ret 8
// engine/CHandleBasedMemoryPool.h (PDB): +0x0C PoolSize. The pool is quiesced with the second
// argument (BeginShutdown(float) @ 0x00A02220 returns false when it cannot be), the new size is
// recorded, and the pool is brought back (EndShutdown @ 0x00A02370). Callee names are the
// manifest's; the float is the time budget BeginShutdown is allowed.
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    bool BeginShutdown(float timeBudget);     // 0x00A02220
    void EndShutdown();                       // 0x00A02370
    bool ResizePool(unsigned long newPoolSize, float timeBudget);
};

bool CHandleBasedMemoryPool_Methods::ResizePool(unsigned long newPoolSize, float timeBudget)
{
    if (!BeginShutdown(timeBudget))
        return false;
    this->PoolSize = newPoolSize;
    EndShutdown();
    return true;
}
