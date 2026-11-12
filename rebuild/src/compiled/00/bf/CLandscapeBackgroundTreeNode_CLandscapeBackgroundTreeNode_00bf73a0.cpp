// CLandscapeBackgroundTreeNode::~CLandscapeBackgroundTreeNode() -- this class's vtable (0x012A8240); delete the patch at +0xa0 (virtual slot 0 with 1) and the load task at +0x98 (its inline destructor: the owned request deleted, the file-block request released @ 0x00BE0400 when issued; then freed through the landscape memory manager, engine 0x01436EA8 -> +0x6b0, FreeVirtual @ 0x00BD9B90); then the CMovableResource base's inline destructor stores its vtable (0x0129B860)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF73A0, length 96 bytes.
// Leaf: ??1CLandscapeBackgroundTreeNode@@QAE@XZ
// Shape: `this` in edi, the task in esi, `push 1 ; call [eax]` twice, `ret`.
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8 (descriptive)
struct CRequestOwner_00bf8c10 {                   // the owned object: a virtual deleting destructor in slot 0
    virtual ~CRequestOwner_00bf8c10();
};
class CEngineStaticMapFileBlockManager { public:
    void ReleaseRequest(void* request);            // 0x00BE0400
};
struct CLandscapeBackgroundLoadTask {              // 16 bytes, no vtable: freed through the landscape memory manager
    CRequestOwner_00bf8c10*             m_pOwner;      // +0x00
    CEngineStaticMapFileBlockManager*   m_pManager;    // +0x04
    void*                               m_pRequest;    // +0x08
    unsigned char                       _pad0c[2];
    bool                                m_requested;   // +0x0e
    unsigned char                       _pad0f;
    ~CLandscapeBackgroundLoadTask()
    {
        if (m_pOwner)
            delete m_pOwner;
        if (m_requested)
            m_pManager->ReleaseRequest(m_pRequest);
    }
    static void operator delete(void* block) { g_pLandscapeRenderer->MemoryManager->FreeVirtual(block); }
};
struct CPatch_00bf73a0 {
    virtual ~CPatch_00bf73a0();                    // slot 0: the deleting destructor
};
class CMovableResource { public:                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }                        // inline: the vtable store
};
class CLandscapeBackgroundTreeNode : public CMovableResource { public:   // vtable 0x012A8240
    unsigned char                  _pad04[0x94];
    CLandscapeBackgroundLoadTask*  m_pLoadTask;    // +0x98
    long                           OffsetIntoFileBlock;
    CPatch_00bf73a0*               m_pPatch;       // +0xa0
    virtual void v00();
    ~CLandscapeBackgroundTreeNode();
};

CLandscapeBackgroundTreeNode::~CLandscapeBackgroundTreeNode()
{
    delete m_pPatch;
    delete m_pLoadTask;
}