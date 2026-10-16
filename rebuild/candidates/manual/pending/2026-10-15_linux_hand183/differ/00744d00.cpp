// Grade: DIFFER(163v166) -- retail keeps the sender pointer's GetPItem in a frame word followed by a constant-false `xor al,al ; jne` (an inlined template test), owner in ebx/ebp, filter slots 8/0xc/0x10 | tried: flat && chain with Bind2 of two one-word filters | next: model the inlined IS_THING_ALIVE<CIntelligentPointer> template that yields the dead test
// CTCPerceiveThing-family FindAlertEvent() -- find the most recent alert message (type 0xC) the owner can see or hear;
// when its sender is alive, an ally of the owner, and the script thing the event names is our tracked thing (+0x0c),
// become directly aware of the perceived thing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00744D00, length 162 bytes (+ int3 padding).
// Label note: catalog "CAIStateGroup_MinionReactToAlert::FindAlertEvent"; the body sits in the CTC region and uses
// +0x04 as the owner thing and +0x0c as an intelligent pointer, and calls SetAsDirectlyAwareOfPerceivedThing
// @ 0x007449B0 on `this` -- treat the class attribution as unverified.
// Callees: the world getter @ 0x00686A80 (`[0x013B8A1C]+24h`, +0x60 = CMessageEventManager),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x00746260,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, AreThingsAllies @ 0x0076D4F0 (__fastcall),
// CGameScriptThing::GetPThing @ 0x006AF520, SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0.
// Shape: two one-word filters and the Bind2 {&first, &second} pair in the 0x10 frame.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CWorld;
struct CMessageEventFilter_IsOfType { long Type; };
struct CMessageEventFilter_CanBeSeenOrHeard { CThing* PPerceiver; };
struct CFilter_Bind2 {
    const CMessageEventFilter_IsOfType* PFirst;
    const CMessageEventFilter_CanBeSeenOrHeard* PSecond;
};

struct CGameScriptThing {
    CThing* GetPThing();                        // 0x006AF520
};
struct CMessageEvent : CGameScriptThing {
    unsigned char _pad[0x48];
    CBaseIntelligentPointerCtor PSender;        // +0x48
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2& filter);   // 0x00746260
};
struct CWorldEvents {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
};

bool __fastcall AreThingsAllies(CThing* a, CThing* b);   // 0x0076D4F0

struct CTCPerceiveAlert {
    void* __vftable;                            // +0x00
    CThing* POwner;                             // +0x04
    unsigned char _pad_0x08[0x4];
    CBaseIntelligentPointerCtor PTracked;       // +0x0c
    CWorldEvents* GetPWorld();                  // 0x00686A80
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
    void FindAlertEvent();
};

void CTCPerceiveAlert::FindAlertEvent()
{
    CThing* owner = POwner;
    CMessageEventFilter_IsOfType isAlert;
    isAlert.Type = 0xc;
    CMessageEventFilter_CanBeSeenOrHeard canPerceive;
    canPerceive.PPerceiver = owner;
    CFilter_Bind2 filter;
    filter.PFirst = &isAlert;
    filter.PSecond = &canPerceive;
    CMessageEvent* ev = GetPWorld()->PMessageEventManager->FindMostRecentMessage(filter);
    if (ev != 0) {
        CBaseIntelligentPointerCtor& sender = ev->PSender;
        if (sender.GetPItem() != 0 && sender.GetPItem() != 0 && !(sender.GetPItem()->m_flags91 & 1) &&
            AreThingsAllies(sender.GetPItem(), owner) && ev->GetPThing() == PTracked.GetPItem())
            SetAsDirectlyAwareOfPerceivedThing();
    }
}
