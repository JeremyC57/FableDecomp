// CLandscapeBackgroundPatch::operator new(unsigned int size, CMovableResource* owner) -- class allocator: takes the block from the landscape memory manager (CEngineLandscapeMemoryManager::AllocVirtual) with the owning movable resource
// Leaf: ??2CLandscapeBackgroundPatch
// Retail: Fable.exe @ 0x00BE5BB0, 22 bytes, `__fastcall` static (size in ecx, owner in edx), out of line. Catalog label
// "_Cons_val<pair<unsigned long,CAnimationEntry>>" is wrong (ten byte-identical class allocators share it; see
// GOTCHAS chunk 152 and landed 0x00BDE600 / 0x00BF6530). Evidence: callers CLandscapeBackgroundTreeNode::BuildDefaultSection 0x00BF6FD0 and ProcessLoadTask 0x00BF8253/0x00BF85D3 allocate 0x11C bytes and run CLandscapeBackgroundPatch ctors (vtable 0x012A803C).
// Callee: CEngineLandscapeMemoryManager::AllocVirtual(long, CMovableResource*) 0x00BD9AF0 on the engine global
// 0x01436EA8 (+0x6b0), as modelled in landed 0x00BF6530.
// Shape: the manager in a named local makes VC7.1 park the size in eax and load the manager straight into ecx
// (the direct expression loads the global into eax and pushes ecx).
struct CMovableResource;
struct CEngineLandscapeMemoryManager {
    void* AllocVirtual(long size, CMovableResource* owner);   // 0x00BD9AF0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

class CLandscapeBackgroundPatch {
public:
    static void* __fastcall operator new(unsigned int size, CMovableResource* owner);
};

void* __fastcall CLandscapeBackgroundPatch::operator new(unsigned int size, CMovableResource* owner)
{
    CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
    return manager->AllocVirtual(size, owner);
}