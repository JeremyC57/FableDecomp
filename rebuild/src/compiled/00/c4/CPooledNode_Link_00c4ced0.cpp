// CPooledNode::Link(CPooledNode* prev, CPooledNode* next) -- hook this node in between prev and next (either may be null)
// Leaf: ?Link@CPooledNode@@QAEXPAU1@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4CED0, 31 bytes, __thiscall `ret 8`: mov edx,[esp+4] ; test edx,edx ; je a ; mov [edx+8],ecx ; a: mov eax,[esp+8] ; test eax,eax ; je b ; mov [eax+4],ecx ; b: mov [ecx+4],edx ; mov [ecx+8],eax ; ret 8
// Was landed as WordWrap_SetCallback on a Node {cb, prev, next} stand-in (manifest, propagated; FINDINGS chunk 438: "pooled node Link(prev,next)"). Callers: CNodePool::Init 0x00C3C8E9, PushActive 0x00C3C9C0, PushFree 0x00C3CA10.
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

void CPooledNode::Link(CPooledNode* prev, CPooledNode* next)
{
    if (prev)
        prev->m_pNext = this;
    if (next)
        next->m_pPrev = this;
    m_pPrev = prev;
    m_pNext = next;
}
