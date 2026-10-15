// CCombatSequence_WillForcePush::OnCreate() -- queue a new CCreatureAction_ForcePushInto(owner) through a counted pointer, then run the base OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090A550, 156 bytes.
// Callees: operator new 0x00BFEA1A, CCombatActionBase::CCombatActionBase 0x00858030, CCountedPointer::Reset
//          0x00905FD0, AddAction 0x008B0B10, base OnCreate 0x008B0650, operator delete 0x00BFE9BC.
//
// Shape: the counted-pointer OnCreate family (landed CCombatSequence_TrollRockPursue 0x00907BE0) with
// the action's constructor INLINED: CCreatureAction_ForcePushInto (0x118 bytes, vtable 0x012783D4)
// calls CCombatActionBase(owner, 150, CSoundPair()) -- the zeroed two-dword sound pair is a stack
// temporary bound to a const reference (no destructor) -- then stores its own vtable and sets the
// byte at +0x114. Because a zero is needed five times (counted pointer, sound pair, the release
// compare) VC keeps it in ebx. The new block lives in esi across the base call, `this` in edi.
// The engine header for the sequence only carries a flattened base blob, so it is modelled locally.
struct CThingCreatureBase;

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    unsigned char _body[0x110];
};

struct CCreatureAction_ForcePushInto : CCombatActionBase {
    bool m_bPushInto;                       // +0x114
    CCreatureAction_ForcePushInto(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 150, CSoundPair()) { m_bPushInto = true; }
    virtual void v00();                     // vtable 0x012783D4
};

struct CCountedPointer {
    CCombatActionBase* m_pObject;           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer& operator=(CCombatActionBase* object) { Reset(object); return *this; }
    void Reset(CCombatActionBase* object);  // 0x00905FD0
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
    CThingCreatureBase* m_pOwner;           // +0x1c
    void* m_pTarget;                        // +0x20
    void AddAction(const CCountedPointer& action);   // 0x008B0B10
    virtual void OnCreate();                // 0x008B0650
};

struct CCombatSequence_WillForcePush : CCombatSequenceBase_008b0650 {
    virtual void OnCreate();
};

void CCombatSequence_WillForcePush::OnCreate()
{
    CCountedPointer action;
    action = new CCreatureAction_ForcePushInto(m_pOwner);
    AddAction(action);
    CCombatSequenceBase_008b0650::OnCreate();
}