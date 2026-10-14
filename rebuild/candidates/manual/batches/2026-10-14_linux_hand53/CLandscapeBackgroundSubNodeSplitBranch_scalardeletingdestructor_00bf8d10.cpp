// CLandscapeBackgroundSubNodeSplitBranch::`scalar deleting destructor'(unsigned int flags) -- destroy the two embedded tree nodes at +0xec and +4 (0x00BF73A0), the CMovableResource base's inline destructor stores its vtable (0x0129B860), then free this branch through the landscape memory manager (engine 0x01436EA8 -> +0x6b0, FreeVirtual @ 0x00BD9B90) for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF8D10, length 64 bytes.
// Leaf: ??_GCLandscapeBackgroundSubNodeSplitBranch@@QAEPAXI@Z
// Shape: the derived vtable store folds into the base's; the class's own `operator delete` frees through the landscape memory manager, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CEngineLandscapeMemoryManager {
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
struct CLandscapeBackgroundTreeNode {              // 0xe8 bytes
    unsigned char _body[0xe8];
    ~CLandscapeBackgroundTreeNode();               // 0x00BF73A0
};
struct CMovableResource {                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }                        // inline: the vtable store
};
struct __declspec(novtable) CLandscapeBackgroundSubNodeSplitBranch : CMovableResource {
    CLandscapeBackgroundTreeNode m_nodeA;          // +0x04
    CLandscapeBackgroundTreeNode m_nodeB;          // +0xec
    static void operator delete(void* block) { g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(block); }
    ~CLandscapeBackgroundSubNodeSplitBranch() { }
};

void Kill_00bf8d10(CLandscapeBackgroundSubNodeSplitBranch* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
