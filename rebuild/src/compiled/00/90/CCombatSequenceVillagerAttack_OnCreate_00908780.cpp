// CCombatSequence_VillagerAttack::OnCreate() -- counted pointer to a new CCombatAction_0097cf30(m_1c, m_20) (0x114 bytes) added through AddAction @ 0x008B0B10, then the base OnCreate @ 0x008B0650; the temporary is released inline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00908780, oracle length 130 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; push esi ; push 0x114 ; mov esi,ecx ; mov dword ptr [esp+8],0 ; mov dword ptr [esp+0Ch],0 ; call operator new ; add esp,4 ; test eax,eax ; je Z ; mov ecx,[esi+0x20] ; mov edx,[esi+0x1c] ; push ecx ; push edx ; mov ecx,eax ; call CCombatAction_0097cf30::CCombatAction_0097cf30 ; jmp J ; Z: xor eax,eax ; J: push eax ; lea ecx,[esp+8] ; call CCountedPointer::Reset ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call AddAction ; mov ecx,esi ; call CCombatSequenceBase_008b0650::OnCreate ; mov eax,[esp+8] ; test eax,eax ; pop esi ; je E ; dec dword ptr [eax] ; mov eax,[esp+4] ; cmp dword ptr [eax],0 ; jne E ; mov ecx,[eax+8] ; call [eax+4] ; mov ecx,[esp+4] ; push ecx ; call operator delete ; add esp,4 ; E: add esp,8 ; ret
// Virtual OnCreate of a combat sequence: a new CCombatAction_0097cf30 (0x114 bytes, constructor @ 0x0097CF30
// over the sequence's two pointer members at +0x1c and +0x20) is wrapped in a counted
// pointer -- default-constructed (both fields zeroed, the stores scheduled before the
// allocation) and assigned through the out-of-line Reset @ 0x00905FD0, which allocates the
// count block -- handed to AddAction @ 0x008B0B10 (its label
// `std::vector::push_back` is propagated: the callee also bumps a count at +8), and the base
// OnCreate @ 0x008B0650 runs (its manifest label is propagated). The temporary's destructor is
// inlined: decrement the block's count and, at zero, call its destroy function with the object
// in ecx and delete the block; the stores nulling a dead local are elided. The sequence class
// is this body's manifest module; the action class is named by its constructor when the label
// is unique. One of 18 bodies of this shape.
struct CThingCreatureBase;
struct CThing;

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};

struct CCombatActionBase {
    virtual void v00();
};

struct CCombatAction_0097cf30 : CCombatActionBase {
    unsigned char _body[0x110];        // sizeof(CCombatAction_0097cf30) = 0x114
    CCombatAction_0097cf30(CThingCreatureBase* owner, CThing* target);   // 0x0097CF30
};

struct CCountedPointer {
    CCombatActionBase* m_pObject;          // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(CCombatActionBase* object) { Reset(object); return *this; }
    void Reset(CCombatActionBase* object); // 0x00905FD0
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct CCombatSequenceBase_008b0650 {
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_1c;               // +0x1c owner
    CThing* m_20;                           // +0x20 target
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();               // 0x008B0650
};

struct CCombatSequence_VillagerAttack : CCombatSequenceBase_008b0650 {
    virtual void OnCreate();
};

void CCombatSequence_VillagerAttack::OnCreate()
{
    CCountedPointer action;                // zeroed before the allocation (GOTCHAS: ctor-argument form zeroes after)
    action = new CCombatAction_0097cf30(m_1c, m_20);
    AddAction(action);
    CCombatSequenceBase_008b0650::OnCreate();
}