// CEngineLandscapePatch::`scalar deleting destructor'(unsigned int flags) -- the inlined destructor (Clear, release of the counted resource pointer at +0x40/+0x44, CMovableResource vtable) then, for flag bit 0, frees the patch through the landscape memory manager
// Leaf: ??_G
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF5D30, length 100 bytes; the manifest's "~CEngineLandscapePatch" label is the
// plain destructor's name propagated onto this deleting twin.
// Callees: CEngineLandscapePatch::Clear @ 0x00BF38F0 (right after the CEngineLandscapePatch vtable store 0x012A8200), the
// count block's `__fastcall` destroy function at +4 (object at +8) when the count reaches zero, operator delete @ 0x00BFE9BC
// for the block, CEngineLandscapeMemoryManager::FreeVirtual @ 0x00BD9B90 (engine 0x01436EA8 -> +0x6B0) for the patch.
// Shape: the body of the landed plain destructor @ 0x00BF3A40 inlined (the release re-reads the block pointer after the
// decrement), then VC's `test byte ptr [esp+8],1` is hoisted above the zero stores and the CMovableResource vtable store
// (0x0129B860); the class's own inline `operator delete` frees through the landscape manager. This TU's class has no
// operator delete[] (the vtable's own slot 0 is the `??_E` @ 0x00BDD3E0 from a TU that has one), so VC emits `??_G`.
// Both destructors must be `__forceinline` (GOTCHAS 24i): a plain inline counted-pointer destructor stays out of line, and a
// plain inline virtual destructor makes `??_G` collapse to `call ??1 ; FreeVirtual`.
extern "C" void __cdecl operator_delete_00bfe9bc(void* block);   // 0x00BFE9BC

struct CEngineLandscapeMemoryManager {
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

struct SCountBlock {                               // the counted pointer's shared block
    long         m_count;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);        // +0x04
    void*        m_pObject;                        // +0x08
};

struct CCountedResourcePointer {
    void*        m_pObject;                        // +0x00
    SCountBlock* m_pBlock;                         // +0x04
    __forceinline ~CCountedResourcePointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_count;
            if (m_pBlock->m_count == 0) {
                m_pBlock->m_pfnDestroy(m_pBlock->m_pObject);
                operator_delete_00bfe9bc(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};

struct CMovableResource {                          // vtable 0x0129B860
    virtual ~CMovableResource() { }
};

struct CEngineLandscapePatch : CMovableResource {  // vtable 0x012A8200
    unsigned char           _pad04[0x3c];          // +0x04 .. +0x40
    CCountedResourcePointer m_resource;            // +0x40
    void Clear();                                  // 0x00BF38F0
    __forceinline virtual ~CEngineLandscapePatch() { Clear(); }
    static void operator delete(void* block) { g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(block); }
};

// VC7.1 emits the class's vtable and scalar deleting destructor in a TU that constructs it; this helper is that construction.
void* MakeOne()
{
    return new CEngineLandscapePatch;
}