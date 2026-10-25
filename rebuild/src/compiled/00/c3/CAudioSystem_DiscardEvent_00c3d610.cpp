// CAudioSystem::DiscardEvent(CAudioEvent* event) -- give a pooled event back: notify it (slot 4), take it off the active chain of the pool its kind selects (slot 1: +0x30, else +0x1c), then park it on that pool's free chain when recycling is enabled or delete it
// Leaf: ?DiscardEvent@CAudioSystem@@QAEXPAUCAudioEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D610, 75 bytes (+ int3 padding), __thiscall `ret 4`:
//   push esi ; mov esi,[esp+8] ; mov eax,[esi] ; push edi ; mov edi,ecx ; mov ecx,esi ; call [eax+10h] ;
//   mov edx,[esi] ; mov ecx,esi ; call [edx+4] ; test al,al ; je a ; add edi,30h ; jmp b ; a: add edi,1Ch ;
//   b: push esi ; mov ecx,edi ; call CNodePool::RemoveActive ; mov eax,[edi+0Ch] ; test eax,eax ; jl del ;
//   push esi ; mov ecx,edi ; call CNodePool::PushFree ; ... ret 4 ; del: mov eax,[esi] ; push 1 ; mov ecx,esi ;
//   call [eax] ; ... ret 4
// Naming (sweep1, 2026-10-25): was landed as CCombatActionManager_00c3d610::RecycleAction (manifest
//   "CCombatSequenceBase::HandleAction", propagated; FINDINGS chunk 438: libpk audio event code). `this` is the audio
//   system singleton 0x0143BB58: the libpk API thunk 0x00C35980 (`mov ecx,0143BB58h ; jmp 0x00C3D610`, called from
//   0x00C27AAB / 0x00C496B4) reaches it, and agent437's AudioSystem_* wrappers name that global CAudioSystem.
//   CAudioEvent and its slot names are descriptive (no PDB covers libpk); the other callers are the event factories
//   0x00C3DB46 / 0x00C3DC22 (discarding an event whose set-up failed).
// Shape: the pool address is selected into edi; `delete` of the polymorphic node emits no null test.
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

struct CAudioEvent : CPooledNode {              // a pooled libpk / GameCODA event
    virtual bool UsesSecondaryPool();           // slot 1 (+4): selects the +0x30 pool
    virtual void v02();
    virtual void v03();
    virtual void OnDiscard();                   // slot 4 (+0x10)
};

struct CAudioSystem {                           // the libpk audio system singleton (0x0143BB58)
    unsigned char _pad00[0x1c];
    CNodePool m_primaryPool;                    // +0x1c
    CNodePool m_secondaryPool;                  // +0x30
    void DiscardEvent(CAudioEvent* event);
};

void CAudioSystem::DiscardEvent(CAudioEvent* event)
{
    event->OnDiscard();
    CNodePool* pool;
    if (event->UsesSecondaryPool())
        pool = &m_secondaryPool;
    else
        pool = &m_primaryPool;
    pool->RemoveActive(event);
    if (pool->m_freeCount >= 0)
        pool->PushFree(event);
    else
        delete event;
}
