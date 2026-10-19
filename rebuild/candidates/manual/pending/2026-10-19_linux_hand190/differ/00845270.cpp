// CCreatureAction_ShowItemToThing::PerformAction() -- when the target thing and the shown item are both present and alive and the target has an entity-events component (0x42), add a new CEventShownItem(owner, item) to it
// Grade: DIFFER(219v212) -- retail keeps one push-ecx word for the key/out pointer, shrink-wraps ebx/ebp and reuses bl=1 for both dead-bit tests (mov cl,[eax+91h] ; mov bl,1 ; test bl,cl); ours opens sub esp,8, spills this and tests with immediates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845270, length 212 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x42 =
// CTCEntityEvents, bit 2 of +0x28), operator new @ 0x00BFEA1A (0x18 bytes), NEntityEvents::CEventShownItem::CEventShownItem
// (CThing*, CThing*) @ 0x0084AEE0 (vtable 0x012723D8; manifest label "CEventDied" is a propagation), CTCEntityEvents::AddEvent
// (CEventBase*) @ 0x008113C0.
// Shape: one `&&` chain: target present, target not dead (thing byte +0x91 bit 0; VC keeps the 1 in bl for the second
// test), item present and alive, then the out-parameter GetInterface on the target; the `new` null test selects 0 and
// the AddEvent call is duplicated into both arms.
// Layout: owner CIntelligentPointer +8, target +0xa8, item +0xb0 (retail CCreatureActionBase is 0xa8).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
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
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CEventBase { };
struct CEventShownItem : CEventBase {
    unsigned char _pad[0x18];
    CEventShownItem(CThing* shower, CThing* item);   // 0x0084AEE0
};
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);           // 0x008113C0
};

static inline bool IsAlive(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

class CCreatureActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

class CCreatureAction_ShowItemToThing : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PTarget;            // +0xa8
    CBaseIntelligentPointer PItem;              // +0xb0
    virtual void PerformAction();
};

void CCreatureAction_ShowItemToThing::PerformAction()
{
    CTCEntityEvents* events;
    if (IsAlive(PTarget) && IsAlive(PItem) && PTarget.GetPItem()->GetInterface(&events, 0x42))
        events->AddEvent(new CEventShownItem(PCreature.GetPItem(), PItem.GetPItem()));
}
