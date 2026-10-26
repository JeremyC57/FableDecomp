// CEngineLandscapeMemoryManager::FreeVirtual(void* block) -- returns a block to the global CMemoryAllocator under the allocator lock and, when the free-when-empty flag is set and nothing is left allocated, destroys the global allocator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9B90, length 97 bytes (`ret 4`; `this` unused).
// Callees: GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010, CMemoryAllocator::InternalDeallocate @ 0x009F2920,
// the allocator's allocation-count query @ 0x009F1F10 (manifest "CWorld::DrawGetEnvironment", propagated), ~CMemoryAllocator
// @ 0x009F2310 and the class `operator delete` @ 0x009F1F90 (`__fastcall`).
// Globals: the global allocator pointer @ 0x013CAA90 (as the landed AllocVirtual 0x00BD9AF0) and the byte flag @ 0x013CAA94.
// Shape: `delete g` through a class `static void __fastcall operator delete(void*)` (GOTCHAS 24h: `mov ecx,[g] ; test ; mov esi,ecx ;
// call dtor ; mov ecx,esi ; call`), the global re-read after each call, then the explicit null store.
class CMemoryAllocator { public:
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10 (manifest "CWorld::DrawGetEnvironment", propagated)
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* p);   // 0x009F1F90
};

void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010

extern CMemoryAllocator* g_pGlobalAllocator_013caa90;   // 0x013CAA90
extern bool g_bFreeGlobalAllocatorWhenEmpty_013caa94;   // 0x013CAA94

class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);
};

void CEngineLandscapeMemoryManager::FreeVirtual(void* block)
{
    if (block) {
        GFEnterAllocatorLock();
        if (g_pGlobalAllocator_013caa90) {
            g_pGlobalAllocator_013caa90->InternalDeallocate(block);
            if (g_bFreeGlobalAllocatorWhenEmpty_013caa94 && g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
        GFLeaveAllocatorLock();
    }
}