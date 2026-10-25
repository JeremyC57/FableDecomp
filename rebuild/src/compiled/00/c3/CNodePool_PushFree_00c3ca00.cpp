// CNodePool::PushFree(CPooledNode* node) -- link the node at the head of the free chain and count it
// Leaf: ?PushFree@CNodePool@@QAEXPAUCPooledNode@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CA00, 36 bytes, __thiscall `ret 4`: push esi ; mov esi,ecx ; mov eax,[esi+8] ; push edi ; mov edi,[esp+0Ch] ; push eax ; push 0 ; mov ecx,edi ; call CPooledNode::Link ; mov eax,[esi+0Ch] ; mov [esi+8],edi ; inc eax ; pop edi ; mov [esi+0Ch],eax ; pop esi ; ret 4
// Was landed as CCombatActionPool_00c3c9b0::PushFree(CPooledCombatAction*) (manifest "CCombatSequenceBase::PushAction", propagated). Callers: the discard paths 0x00C3CE06 (RecycleGlobalNode), 0x00C3D644 (CAudioSystem::DiscardEvent), 0x00C3D685 / 0x00C3D6C5 (agent438's DiscardMorphEvent / DiscardScreenEvent), 0x00C4D16D.
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

void CNodePool::PushFree(CPooledNode* node)
{
    node->Link(0, m_pFreeHead);
    m_pFreeHead = node;
    ++m_freeCount;
}
