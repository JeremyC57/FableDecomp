// CSoundManagerLH::StopSound() -- stop a playing sample id on every 3-D emitter, then on the 2-D emitter (their slot 9 StopSpecificSample)
// Name (agentSND, 2026-11-10): vtable slot 14 (+0x38); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was CEngineManager_00a3ebf0::RemoveMap.
// Leaf: StopSound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3EBF0, length 49 bytes.
// Callees: virtual slot 9 (+0x24) on each listener (the object at node+8) and on the object at +0x44. Shape: the argument is loaded into ebx before the
// loop; the node runs in esi, the listener straight into ecx; the loop test re-reads the sentinel (`cmp esi,[edi+4Ch]`); the final call reuses ebx.
// The chunk label "CEngineManager::RemoveMap" is propagated (the CEngineManager list sits at +4); the class is named by address.
struct CASoundEmitter {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void StopSound(void* map);             // slot 9 (+0x24)
};

struct SListenerNode {
    SListenerNode*         m_pNext;
    SListenerNode*         m_pPrev;
    CASoundEmitter* m_pListener;            // +0x08
};

struct CSoundManagerLH {
    unsigned char          _pad0[0x44];
    CASoundEmitter* m_pOwner;               // +0x44
    unsigned char          _pad1[4];
    SListenerNode*         m_pListeners;           // +0x4c (sentinel node)
    void StopSound(void* map);
};

void CSoundManagerLH::StopSound(void* map)
{
    for (SListenerNode* node = m_pListeners->m_pNext; node != m_pListeners; node = node->m_pNext)
        node->m_pListener->StopSound(map);
    m_pOwner->StopSound(map);
}