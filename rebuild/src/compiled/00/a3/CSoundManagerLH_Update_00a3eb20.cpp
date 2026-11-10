// CSoundManagerLH::Update() -- per-frame update: the base CASoundManager::Update, then the 2-D emitter and every 3-D emitter on the list (their slot 14 Update)
// Name (agentSND, 2026-11-10): vtable slot 3 (+0xC); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was CListOwner_00a3eb20::ResetAll.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EB20, length 64 bytes.
// Leaf: ?Update@CSoundManagerLH@@UAEXXZ
// Callees: 0x00991670 (thiscall, no arguments; the manifest label is garbage). The chunk label (CNavigatorManager::InterruptAllNavigators) is propagated; named by address. Shape: `this` in edi, the node in esi, the loop head aligned with a 6-byte `lea`, `ret`.
struct CASoundEmitter {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void Update();                          // slot 14 (+0x38)
};
struct SEmitterNode {
    SEmitterNode*        m_pNext;
    SEmitterNode*        m_pPrev;
    CASoundEmitter*  m_pObject;              // +0x08
};

struct CASoundManager {                            // vptr +0x00 (slot 3 = Update, engine/CASoundManager.h)
    virtual void Update();                         // 0x00991670 (CASoundManager::Update, called qualified)
    unsigned char          _pad04[0x40];
};
struct CSoundManagerLH : CASoundManager {
    CASoundEmitter*  m_p2DEmitter;                // +0x44
    long                   m_48;
    SEmitterNode*        m_pList;                // +0x4c: the sentinel
    virtual void Update();
};

void CSoundManagerLH::Update()
{
    CASoundManager::Update();
    m_p2DEmitter->Update();
    for (SEmitterNode* node = m_pList->m_pNext; node != m_pList; node = node->m_pNext)
        node->m_pObject->Update();
}