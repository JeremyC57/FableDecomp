// CLocalDetailPrimitiveRepeatedMesh::OnPostRelocation() -- after this mesh moved, re-register it as the owner of its five landscape-memory blocks through the engine's landscape memory manager (0x01436EA8 -> +0x6b0): +0x50 and +0x48 with 0x00BD9AB0, +0x54, +0x44 and +0x4c with 0x00BD9AE0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9B40, length 78 bytes.
// Leaf: OnPostRelocation
// Callees: 0x00BD9AB0 and 0x00BD9AE0 (manager members, `ret 8`; both empty in retail). The chunk label (vfunc_7) is the vtable slot (0x012A82AC slot 7); the body mirrors CLocalDetailPrimitiveMeshZSpriteBatch::OnPostRelocation @ 0x00BF9B90. Shape: the manager in edi, `this` in esi, `ret`.
class CMovableResource;
class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, CMovableResource* owner);    // 0x00BD9AB0 (an empty `ret 8` in retail; the landed 0x00BDD8E0 model calls it too)
    void RebindBufferOwner(void* block, CMovableResource* owner);   // 0x00BD9AE0 (the second empty `ret 8` member, landed as E2::M(int, int))
    void AppendFreeEdgeNode(void* block);          // 0x00BDA090
    void FreeBlock(void* block);                   // 0x00BDA0D0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
class CMovableResource { public:
    virtual void v00();
};
struct CLocalDetailPrimitiveRepeatedMesh : CMovableResource {   // vtable 0x012A82AC
    unsigned char _pad04[0x40];
    void*         m_pBuffer44;                     // +0x44: landscape-memory blocks (cleared by the constructor @ 0x00BFAA80)
    void*         m_pBlock48;                      // +0x48
    void*         m_pBuffer4c;                     // +0x4c
    void*         m_pBlock50;                      // +0x50
    void*         m_pBuffer54;                     // +0x54
    void OnPostRelocation();
};

void CLocalDetailPrimitiveRepeatedMesh::OnPostRelocation()
{
    CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
    manager->RebindBlockOwner(m_pBlock50, this);
    manager->RebindBufferOwner(m_pBuffer54, this);
    manager->RebindBufferOwner(m_pBuffer44, this);
    manager->RebindBlockOwner(m_pBlock48, this);
    manager->RebindBufferOwner(m_pBuffer4c, this);
}