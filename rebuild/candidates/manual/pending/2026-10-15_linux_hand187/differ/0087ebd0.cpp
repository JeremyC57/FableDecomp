// Grade: DIFFER(218v218) -- register choice only: retail holds &PCreature in ebp (push ebp ; lea ebp,[esi+8]), ours ebx | tried: one shared component local, two component locals | next: lane gap: register allocation (ebx vs ebp)
// CCombatAction_KickThingOnGround::FinishAction() -- when the kicked thing is still alive, unlink kicker and target from each other's interface-0x4D component, then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087EBD0, 218 bytes (224 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type
//   0x4D = bit 13 of +0x28), the component call 0x00770F20 (thiscall, CThing*; manifest "CThing_RemoveFromList"),
//   CCombatActionBase::FinishAction 0x00859A20.
// Layout: CCombatActionBase PCreature +0x08 (landed 0x008C2A80); the kick target CIntelligentPointer at +0xa8 (retail;
//   engine/CCombatAction_KickThingOnGround.h is pads only at these offsets). Thing model as the landed 0x0095D310.
// Shape: IS_THING_ALIVE re-fetches GetPItem for each test; each GetInterface's key lives in the frame; `this` is spilled
//   once to the frame because esi is reused for the map.
// Leaf: FinishAction
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCInterface4D {                         // interface type 0x4D
    void RemoveThing(CThing* thing);            // 0x00770F20
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];          // +0x10
    void FinishAction();                        // 0x00859A20
};

struct CCombatAction_KickThingOnGround : CCombatActionBase {
    CBaseIntelligentPointer PTarget;            // +0xa8
    void FinishAction();
};

void CCombatAction_KickThingOnGround::FinishAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        CTCInterface4D* kickerComponent;
        if (PCreature.GetPItem()->GetInterface(&kickerComponent, 0x4d))
            kickerComponent->RemoveThing(PTarget.GetPItem());
        CTCInterface4D* targetComponent;
        if (PTarget.GetPItem()->GetInterface(&targetComponent, 0x4d))
            targetComponent->RemoveThing(PCreature.GetPItem());
    }
    CCombatActionBase::FinishAction();
}
