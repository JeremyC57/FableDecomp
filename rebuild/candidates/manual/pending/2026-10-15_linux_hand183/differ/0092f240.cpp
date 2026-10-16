// Grade: DIFFER(195v193) -- filter scheduling: retail stores Bind2.PFirst before the IsOriginatedByThing ctor call and IsOfType.Type after it, keeps the message in edi; later the creature's bitset base held in eax (ours ecx) | tried: filter temporaries (4-byte, then 8-byte second filter -> frame 0x14 fixed), named filter/Bind2 locals, named manager local
// CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed() -- withdraw the creature's own pending interaction
// message (type 0x14) and, if it has the interface-0x10 talk component, play TAVERN_GAME_INCORRECT.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092F240, length 194 bytes (+ int3 padding).
// Callees: CMessageEventFilter_IsOriginatedByThing(CThing*) @ 0x006AF290, the world getters @ 0x008FCFA0 and
// @ 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated), CMessageEventManager::
// FindMostRecentMessage<CFilter_Bind2<IsOfType, IsOriginatedByThing>> @ 0x008D46D0, CMessageEventManager::RemoveMessage
// @ 0x00493B5B, CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + entry lookup, type 0x10 = bit 16 of the dword at
// +0x20), CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_INCORRECT" @ 0x0126F6AC), the component's play call
// @ 0x0066BD00 (manifest "CSoundInfo::PlaySound" is propagated), ~CCharString @ 0x0099EAE0.
// Shape: the Bind2 {&first, &second} temporary over two filter temporaries (the second built by its out-of-line ctor,
// whose returned `this` is stored as the second pointer), a NAMED CCharString for the sound name.
// The engine header flattens the base, so the class is modelled locally.
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

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {   // 8 bytes, filled by its out-of-line ctor
    void* __vftable;
    CThing* POriginator;
    CMessageEventFilter_IsOriginatedByThing(CThing* thing);   // 0x006AF290
};
struct CFilter_Bind2 {
    const CMessageEventFilter_IsOfType* PFirst;
    const CMessageEventFilter_IsOriginatedByThing* PSecond;
    CFilter_Bind2(const CMessageEventFilter_IsOfType& a, const CMessageEventFilter_IsOriginatedByThing& b)
        : PFirst(&a), PSecond(&b) { }
};

struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2& filter);   // 0x008D46D0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
};

struct CTCTalkReaction {                        // the interface-0x10 component
    void PlayReaction(const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066BD00
};

struct CAIStateGroup_MinionReactToPlayerInteraction : CAIStateGroupBase {
    unsigned long NextReactionFrame[5];         // +0x14
    long ReactionType;                          // +0x28 EInteractionReactionType
    bool WaitingForConfirmation;                // +0x2c
    bool DidTraderInteraction;                  // +0x2d
    CWorld* GetPWorldA();                       // 0x008FCFA0
    CWorld* GetPWorld();                        // 0x008FCFC0
    void InteractionFailed();
};

void CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed()
{
    CMessageEvent* message = GetPWorldA()->PMessageEventManager->FindMostRecentMessage(
        CFilter_Bind2(CMessageEventFilter_IsOfType(0x14), CMessageEventFilter_IsOriginatedByThing(PBrain->PCreature)));
    if (message != 0) {
        CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
        manager->RemoveMessage(message);
        if (PBrain->PCreature->HasTC(0x10)) {
            CTCTalkReaction* talk = (CTCTalkReaction*)PBrain->PCreature->FindEntry(0x10)->component;
            CCharString name("TAVERN_GAME_INCORRECT");
            talk->PlayReaction(name, true, false, false, -1);
        }
    }
}
