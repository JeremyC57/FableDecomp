// CPredicateChain_00c27070::CPredicateChain_00c27070(void*) -- EH-framed constructor: the base built out of line (@ 0x00C1C810, state 0), the member at +0x18 zeroed inline (words +4..+0xc, state 1), the member at +0x28 built with (0, 30000) (@ 0x00C29FC0, state 2), the node pointer at +0x2c zeroed, then a 0x20-byte node allocated (`new`, state 3) and built inline (base @ 0x00C2A230 with 0x3000, vtable 0x012B4024, the argument, the two members and `this` at +0x10..+0x1c), stored at +0x2c and registered (@ 0x00C2A270)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27070, oracle length 187 bytes. Built under /EHsc.
// Retail bytes:  ... call CBase::CBase ; xor esi,esi ; lea ebp,[edi+18h] ; mov [esp+20h],esi ; mov [ebp+4],esi ; mov [ebp+8],esi ; mov [ebp+0Ch],esi ; push 7530h ; lea ebx,[edi+28h] ; push esi ; mov ecx,ebx ; mov byte ptr [esp+28h],1 ; call CTimer::CTimer ; push 20h ; mov byte ptr [esp+24h],2 ; mov [edi+2Ch],esi ; call operator new ; mov esi,eax ; add esp,4 ; mov [esp+14h],esi ; test esi,esi ; mov byte ptr [esp+20h],3 ; je null ; push 3000h ; mov ecx,esi ; call CNodeBase::CNodeBase ; mov eax,[esp+28h] ; mov dword ptr [esi],012B4024h ; mov [esi+10h],eax ; mov [esi+14h],ebx ; mov [esi+18h],edi ; mov [esi+1Ch],ebp ; mov ecx,esi ; jmp reg ; null: xor ecx,ecx ; reg: mov byte ptr [esp+20h],2 ; mov [edi+2Ch],ecx ; call CNode::Register ; ...
// The twin of 0x00C26C10 (a different node vtable). The class stores no vtable of its own (the base's store is the only one). The manifest label
// is a description; the classes are named by address.
struct CPredicateChain_00c27070;
struct CPredicateBase_00c27070 {
    unsigned char _body[0x18];
    CPredicateBase_00c27070();             // 0x00C1C810
    ~CPredicateBase_00c27070();
};

struct CSlotList_00c27070 {                // +0x18, 16 bytes
    long m_0;
    void* m_pFirst;                        // +4
    void* m_pLast;                         // +8
    void* m_pEnd;                          // +0xc
    CSlotList_00c27070() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CSlotList_00c27070();
};

struct CTimer_00c27070 {                   // +0x28, one word
    long m_0;
    CTimer_00c27070(long start, long period);   // 0x00C29FC0
    ~CTimer_00c27070();
};

struct CNodeBase_00c27070 {                // 16 bytes, polymorphic
    virtual void v00();
    unsigned char _body[0xc];
    CNodeBase_00c27070(long capacity);     // 0x00C2A230
};

struct CChainNode_00c27070 : CNodeBase_00c27070 {   // vtable 0x012B4024, 0x20 bytes
    void* m_pTarget;                       // +0x10
    CTimer_00c27070* m_pTimer;             // +0x14
    CPredicateChain_00c27070* m_pOwner;    // +0x18
    CSlotList_00c27070* m_pSlots;          // +0x1c
    CChainNode_00c27070(void* target, CTimer_00c27070* timer, CPredicateChain_00c27070* owner, CSlotList_00c27070* slots)
        : CNodeBase_00c27070(0x3000), m_pTarget(target), m_pTimer(timer), m_pOwner(owner), m_pSlots(slots) { }
    void Register();                       // 0x00C2A270
};

struct CPredicateChain_00c27070 : CPredicateBase_00c27070 {
    CSlotList_00c27070 m_slots;            // +0x18
    CTimer_00c27070 m_timer;               // +0x28
    CChainNode_00c27070* m_pNode;          // +0x2c
    CPredicateChain_00c27070(void* target);
};

CPredicateChain_00c27070::CPredicateChain_00c27070(void* target)
    : CPredicateBase_00c27070(), m_slots(), m_timer(0, 30000), m_pNode(0)
{
    m_pNode = new CChainNode_00c27070(target, &m_timer, this, &m_slots);
    m_pNode->Register();
}