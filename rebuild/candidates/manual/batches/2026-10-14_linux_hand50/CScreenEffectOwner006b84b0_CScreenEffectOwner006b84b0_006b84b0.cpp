// CScreenEffectOwner_006b84b0::CScreenEffectOwner_006b84b0(long id, void* pOwner) -- a non-polymorphic record: the id word at +4 from the initialiser list, the priority stack at +0x90 constructed, then two flag bytes (+0x78, +0x79) cleared and the owner pointer stored at +0 in the body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B84B0, length 39 bytes (+ int3 padding).
// Callees: CScreenEffectPriorityStack::CScreenEffectPriorityStack @ 0x00881370 (on the member at +0x90).
// The chunk label "CInputProcessGameBase::AddToInterface" is propagated; the body returns `this` (`mov eax,esi ; ret 8`) and stores the
// second argument into the first word, so this is a constructor of a non-polymorphic class. Shape: the id is stored before the member
// constructor call (initialiser), the bytes and the owner after it (body), the bytes through one `xor al,al`.
// Leaf: ??0CScreenEffectOwner_006b84b0
struct CScreenEffectPriorityStack {
    unsigned char _body[0x10];
    CScreenEffectPriorityStack();                   // 0x00881370
};
struct CScreenEffectOwner_006b84b0 {
    void*                      m_pOwner;            // +0x00
    long                       m_id;                // +0x04
    unsigned char              _pad08[0x70];        // +0x08
    bool                       m_active;            // +0x78
    bool                       m_dirty;             // +0x79
    unsigned char              _pad7a[0x16];        // +0x7a
    CScreenEffectPriorityStack m_priorityStack;     // +0x90
    CScreenEffectOwner_006b84b0(long id, void* pOwner);
};

CScreenEffectOwner_006b84b0::CScreenEffectOwner_006b84b0(long id, void* pOwner)
    : m_id(id), m_priorityStack()
{
    m_active = false;
    m_dirty = false;
    m_pOwner = pOwner;
}
