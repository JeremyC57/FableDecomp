// CSoundEmitterLH::RemoveInactiveSounds() -- when the sound system is up: erase every active sound whose player state is clear and whose progress has reached 1.0, then record whether any sound is left (+0x19)
// Leaf: RemoveInactiveSounds
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AABCA0, 151 bytes (`ret`).
// Label: the catalog's "CModeHoverBase::RemoveInactiveHoverAnims" is propagated; the layout (+0x04 the sound manager,
// +0x14 the VC6 list of CActiveSound pointers) is CSoundEmitterLH's (landed vfunc_9 0x00AAC400 calls this body by
// that name, and the CSoundEmitterLH vtable 0x0129E6EC lists its siblings 0x00AAC2E0 / 0x00AAC460 / 0x00AAC5E0).
// Callees: the system's slot 6 (+0x18, GetPlayer), the player's slot 28 (+0x70: progress of a handle, state out),
// the list's out-of-line erase(iterator) @ 0x00AACBB0 (iterator by value in the argument area, result by hidden
// pointer), 1.0f @ 0x0122DED8.
// Shape: the sound pointer read before the virtual calls (edi), the sentinel re-read per iteration through the list
// address (ebx), the erase result read back from its frame slot; size() as an inline counting walk, `!= 0` stored.
struct CSoundPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual float GetProgress(long handle, long* state);   // slot 28 (+0x70)
};

struct CSoundSystem {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundPlayer* GetPlayer();      // slot 6 (+0x18)
};

struct CSoundManagerLH {
    unsigned char _00[0x40];
    CSoundSystem* m_pSystem;                // +0x40
};

struct CActiveSound {                       // CSoundEmitterLH::CActiveSound (vtable 0x0129E67C)
    virtual void v00();
    long m_references;                      // +0x04
    long m_handle;                          // +0x08
};

struct SSoundNode {                         // VC6 list node, 16 bytes
    SSoundNode*   next;
    SSoundNode*   prev;
    CActiveSound* sound;                    // +0x08
    long          extra;                    // +0x0c
};

struct CSoundList {                         // list<CCountedPointer<CActiveSound>> (retail: the sentinel pointer)
    struct iterator {
        SSoundNode* m_pNode;
        iterator(SSoundNode* node) : m_pNode(node) { }
        iterator(const iterator& other) : m_pNode(other.m_pNode) { }
    };
    SSoundNode* m_pHead;
    iterator erase(iterator position);      // 0x00AACBB0
    unsigned long size() const
    {
        unsigned long count = 0;
        for (SSoundNode* node = m_pHead->next; node != m_pHead; node = node->next)
            ++count;
        return count;
    }
};

struct CSoundEmitterLH {                    // vtable 0x0129E6EC
    virtual void v00();
    CSoundManagerLH* m_pManager;            // +0x04
    float            m_position[3];         // +0x08
    CSoundList       m_sounds;              // +0x14
    unsigned char    m_18;                  // +0x18
    bool             m_hasSounds;           // +0x19
    void RemoveInactiveSounds();
};

void CSoundEmitterLH::RemoveInactiveSounds()
{
    if (m_pManager->m_pSystem) {
        CSoundList::iterator it(m_sounds.m_pHead->next);
        while (it.m_pNode != m_sounds.m_pHead) {
            CActiveSound* sound = it.m_pNode->sound;
            long state;
            float progress = m_pManager->m_pSystem->GetPlayer()->GetProgress(sound->m_handle, &state);
            if (state == 0 && progress >= 1.0f)
                it = m_sounds.erase(it);
            else
                it.m_pNode = it.m_pNode->next;
        }
        m_hasSounds = m_sounds.size() != 0;
    }
}
