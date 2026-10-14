// CScriptHandleRegistry::CreateNewInstance() -- allocate a 12-byte handle node, register it, hand back its entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C307E0, length 96 bytes.
// Callees: operator new @ 0x00BFEA1A, virtual slot 9 (+0x24) on `this` with the new node.
// Globals: g_pCurrentOwner @ 0x0139C900 (copied into the node), s_nextHandleId @ 0x0143BB44 (post-incremented).
// The manifest's "C3DAnimFileXSequenceChunk::CreateNewInstance" label is propagated noise (same shape as
// the chunk factories: `new` with the inlined constructor, null-checked, then one virtual on `this`).
// The node is {bool +0 = false, SHandleEntry +4 {owner +4, id +8}}; the virtual's result is discarded and
// the address of the embedded entry (`lea eax,[esi+4]`, no null test for a member address) is returned --
// on both sides of the allocation test, so a failed `new` yields the stand-in address 4.
struct SHandleEntry {
    void* m_pOwner;                         // +0 (node +4)
    long m_id;                              // +4 (node +8)
};

extern void* g_pCurrentOwner;               // 0x0139C900
extern long s_nextHandleId;                 // 0x0143BB44

struct SHandleNode {
    bool m_bClaimed;                        // +0
    SHandleEntry m_entry;                   // +4
    SHandleNode() : m_bClaimed(false)
    {
        m_entry.m_pOwner = g_pCurrentOwner;
        m_entry.m_id = s_nextHandleId++;
    }
};

struct CScriptHandleRegistry {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void Register(SHandleNode* node);   // slot 9 (+0x24)
    SHandleEntry* CreateNewInstance();
};

SHandleEntry* CScriptHandleRegistry::CreateNewInstance()
{
    SHandleNode* node = new SHandleNode;
    Register(node);
    return &node->m_entry;
}