// CAnimQueueOwner00aac2e0::StopAllQueuedAnims() -- when the parent thing has an animation owner, stop every queued animation through it, reset the handles, drop the inactive entries and clear the queue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAC2E0, length 90 bytes (+ int3 padding).
// Callees: the parent's animation owner (`[ParentThing+40h]`, virtual slot 6 = GetAnimationPlayer), its slot 0x16 (StopAnim(handle)), RemoveInactiveEntries @ 0x00AABCA0
// (manifest "CModeHoverBase::RemoveInactiveHoverAnims"), list<CCountedPointer<CEntry>>::clear() @ 0x00AACC60 (a void member tail-jumped with
// the list's address in ecx; the manifest "remove_if<...>" prototype is propagated). The chunk label "CDrawQuestInfo::GetMiniGameType" is
// propagated; the body sits among the CModeHoverBase bodies of the 0x00AAxxxx module (RemoveHoverAnimsBySubstring @ 0x00AAC460, PushAnimEntry
// @ 0x00AAC5E0) but its layout (parent thing +4, a list of counted entries at +0x14 whose entry holds a long handle at +8) does not match the
// PDB CModeHoverBase (AnimQueue +0x38, CHoverAnim without a handle), so it is named by address.
// Shape: a speed body with ebp as a general register (`mov ebp,ecx`), a 7-byte `lea esp,[esp]` loop alignment, the animation owner re-read
// from the parent every iteration (after the virtual call), the list pointer in ebx for the end compare and the final tail jump.
struct CAnimationPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21();
    virtual void StopAnim(long handle);            // slot 0x16 (+0x58)
};

struct CAnimationOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CAnimationPlayer* GetAnimationPlayer();   // slot 6 (+0x18)
};

struct CThing {
    unsigned char    _pad[0x40];
    CAnimationOwner* m_pAnimationOwner;            // +0x40
};

struct SAnimEntry {
    unsigned char _pad[8];
    long          m_animHandle;                    // +0x08
};

struct CRetailEntryList {                          // VC6 Dinkumware list<CCountedPointer<SAnimEntry>>: one sentinel pointer
    struct node { node* next; node* prev; SAnimEntry* m_pObject; void* m_pBlock; };
    struct iterator {
        node* m_pNode;
        explicit iterator(node* n) : m_pNode(n) { }
        SAnimEntry* operator*() const { return m_pNode->m_pObject; }
        iterator& operator++() { m_pNode = m_pNode->next; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    node* head;
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    void clear();                                  // 0x00AACC60
};

struct CAnimQueueOwner00aac2e0 {
    unsigned char    _pad[4];
    CThing*          m_pParentThing;               // +0x04
    unsigned char    _pad08[0x14 - 8];
    CRetailEntryList m_animQueue;                  // +0x14
    void RemoveInactiveEntries();                  // 0x00AABCA0
    void StopAllQueuedAnims();
};

void CAnimQueueOwner00aac2e0::StopAllQueuedAnims()
{
    if (m_pParentThing->m_pAnimationOwner != 0) {
        for (CRetailEntryList::iterator it = m_animQueue.begin(); it != m_animQueue.end(); ++it) {
            SAnimEntry* entry = *it;
            m_pParentThing->m_pAnimationOwner->GetAnimationPlayer()->StopAnim(entry->m_animHandle);
            entry->m_animHandle = -1;
        }
        RemoveInactiveEntries();
        m_animQueue.clear();
    }
}
