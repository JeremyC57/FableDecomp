// CNodePool::PopFree() -- take the head off the free chain (advance the head, unlink the node) and drop the free count
// Leaf: ?PopFree@CNodePool@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CA30, 22 bytes, __thiscall `ret`: push esi ; mov esi,ecx ; mov ecx,[esi+8] ; mov eax,[ecx+8] ; mov [esi+8],eax ; call CPooledNode::Unlink ; dec dword ptr [esi+0Ch] ; pop esi ; ret
// Was landed as CCombatActionPool::Touch / CGuiVarTransferFloat::TransferFromGui (manifest, propagated; FINDINGS chunk 438: "Touch" is PopFree). Callers (8): each event factory after GetFree 0x00C3C9A0 when its budget is positive (e.g. 0x00C3CD9A on the global pool 0x0143BC00), before PushActive 0x00C3C9B0.
// Family (sweep1, 2026-10-25): libpk's two-chain node pool (agent438's CNodePool / CPooledNode, whose landed rows
//   Init 0x00C3C8C0, Clear 0x00C3C930, RemoveActive 0x00C3C9E0, RemoveFree 0x00C3CA50 carried address suffixes) -- one
//   name set here: CNodePool {owner, active head +4, free head +8, free count +0xc (negative: no recycling), active
//   count +0x10} over CPooledNode {vtable, prev +4, next +8}. The game names on these rows (CCombatActionPool,
//   CPooledCombatAction, WordWrap, CLinkedItem) are propagated labels (FINDINGS chunk 438). Users: the audio system
//   singleton 0x0143BB58's event pools at +0x1c / +0x30 / +0x44 and the global pool 0x0143BC00. Descriptive names (no
//   PDB covers libpk).
struct CPooledNode {                            // intrusive doubly-linked node; pooled libpk events derive from it
    virtual ~CPooledNode();                     // slot 0 (a discarded node is deleted through it)
    CPooledNode* m_pPrev;                       // +0x04
    CPooledNode* m_pNext;                       // +0x08
    void Link(CPooledNode* prev, CPooledNode* next);   // 0x00C4CED0
    void Unlink();                              // 0x00C4CEF0
};

struct CNodePool {                              // 20 bytes: an active chain and a free chain
    void*        m_pOwner;                      // +0x00
    CPooledNode* m_pActiveHead;                 // +0x04
    CPooledNode* m_pFreeHead;                   // +0x08
    long         m_freeCount;                   // +0x0c (negative: recycling disabled)
    long         m_activeCount;                 // +0x10
    void Init(void* block, long stride, long count);   // 0x00C3C8C0
    void Clear();                               // 0x00C3C930
    CPooledNode* GetFree();                     // 0x00C3C9A0
    void PushActive(CPooledNode* node);         // 0x00C3C9B0
    void RemoveActive(CPooledNode* node);       // 0x00C3C9E0
    void PushFree(CPooledNode* node);           // 0x00C3CA00
    void PopFree();                             // 0x00C3CA30
    void RemoveFree(CPooledNode* node);         // 0x00C3CA50
};

void CNodePool::PopFree()
{
    CPooledNode* node = m_pFreeHead;
    m_pFreeHead = node->m_pNext;
    node->Unlink();
    --m_freeCount;
}
