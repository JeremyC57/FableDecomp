// CVertexBufferChainOwner_00bdd8e0::DestroyProceduralTextureVertexBuffers() -- hands every node of the singly linked chain at +8 (next at +4) to the engine's landscape memory manager together with `this`
// Leaf: DestroyProceduralTextureVertexBuffers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDD8E0, length 43 bytes (48 with padding).
// Callees: the memory manager's dispose member @ 0x00BD9AB0 (`this`-call (ecx), two arguments: node, owner; an empty `ret 8` in retail). Globals: the engine
// @ 0x01436EA8 with its landscape memory manager at +0x6b0 (see CEngineResourceManager::ChangeScene @ 0x00B3B4A0, landed). Shape: the manager is read into
// ebx before the chain's null test (a named local), the loop is bottom-tested with the successor loaded after the call. The chunk label
// (CEngineLandscapePatch::DestroyProceduralTextureVertexBuffers) is propagated: the patch's +8 is a polymorphic layer mesh (0x00BF3790), not a node chain.
struct SVertexBufferNode_00bdd8e0 {
    unsigned char              _pad[4];
    SVertexBufferNode_00bdd8e0* m_pNext;           // +0x04
};

struct CVertexBufferChainOwner_00bdd8e0;

struct CEngineLandscapeMemoryManager {
    void DisposeVertexBuffer(SVertexBufferNode_00bdd8e0* node, CVertexBufferChainOwner_00bdd8e0* owner);   // 0x00BD9AB0
};

struct CEngineGlobal_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemoryManager;   // +0x6b0
};
extern CEngineGlobal_01436ea8* g_pEngine_01436ea8;   // 0x01436EA8

struct CVertexBufferChainOwner_00bdd8e0 {
    unsigned char              _pad[8];
    SVertexBufferNode_00bdd8e0* m_pHead;           // +0x08
    void DestroyProceduralTextureVertexBuffers();
};

void CVertexBufferChainOwner_00bdd8e0::DestroyProceduralTextureVertexBuffers()
{
    CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemoryManager;
    for (SVertexBufferNode_00bdd8e0* node = m_pHead; node; node = node->m_pNext)
        manager->DisposeVertexBuffer(node, this);
}