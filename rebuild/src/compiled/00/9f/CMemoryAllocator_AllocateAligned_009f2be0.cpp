// CMemoryAllocator::AllocateAligned(unsigned long size, long alignment) -- allocate size + alignment + 3 bytes from the global allocator (creating it on first use) under the allocator lock, round the block + 4 up to the power-of-two alignment and keep the real block pointer in the dword just below the aligned pointer (DeallocateAligned reads it back)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2BE0, length 79 bytes.
// Leaf: ?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z
// PDB: ?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z (public static __fastcall: size in ecx, alignment in edx).
// Callees: CMemoryAllocator::Create @ 0x009F2870 (static __fastcall), KERNEL32 EnterCriticalSection (IAT 0x0143FE0C) /
//   LeaveCriticalSection (IAT 0x0143FE10) on the lock @ 0x013CAA70, CMemoryAllocator::InternalAllocate(long, unsigned long, bool)
//   @ 0x009F2A40 (landed; pushed 0, 0, size).
// Globals: the allocator @ 0x013CAA90.
// Shape: the inline static Allocate(size, type) (create-on-demand test before the lock, the InternalAllocate result kept in edi
//   across the leave) followed by the align-up `(block + alignment + 3) & ~(alignment - 1)` (`dec esi ; not esi ; and`).
// Naming (NF161, 2026-11-20, q4196): the static at 0x013CAA90 respelled in place in the row's existing local model to the PDB name
//   ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A (protected static; Ego_r.pdb / FableWin.pdb / Ego_d.pdb data symbol, engine/CMemoryAllocator.h),
//   was ?Allocator@CMemoryAllocator@@1PAV1@A (no such member in any PDB).
#include "fable_compiler.h"
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern unsigned char g_allocatorLock[24];   // 0x013CAA70 (CRITICAL_SECTION)

class CMemoryAllocator {
protected:
    unsigned char _body[0x49c];                       // PDB CMemoryAllocator is 0x49c bytes, no vtable
    void* InternalAllocate(long size, unsigned long type, bool flag);   // 0x009F2A40
    static CMemoryAllocator* PMemoryAllocator;               // 0x013CAA90
    static void __fastcall Create();                  // 0x009F2870
    static void EnterSynchronisation() { EnterCriticalSection(g_allocatorLock); }
    static void LeaveSynchronisation() { LeaveCriticalSection(g_allocatorLock); }
public:
    static FABLE_FORCEINLINE void* Allocate(unsigned long size, unsigned long type)
    {
        if (!PMemoryAllocator)
            Create();
        EnterSynchronisation();
        void* block = PMemoryAllocator->InternalAllocate(size, type, false);
        LeaveSynchronisation();
        return block;
    }
    static void* __fastcall AllocateAligned(unsigned long size, long alignment);
};

void* __fastcall CMemoryAllocator::AllocateAligned(unsigned long size, long alignment)
{
    void* block = Allocate(size + alignment + 3, 0);
    void** aligned = (void**)(((unsigned long)block + alignment + 3) & ~(alignment - 1));
    aligned[-1] = block;
    return aligned;
}