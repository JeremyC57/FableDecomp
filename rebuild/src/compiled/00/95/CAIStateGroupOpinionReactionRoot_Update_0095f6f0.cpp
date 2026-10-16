// CAIStateGroup_OpinionReactionRoot::Update() -- run the current update member function (marking the opinion
// component's reaction as done when it succeeds), then wait (yielding) while the creature is still talking; always
// finish the update; bail straight out when the state group is no longer valid.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F6F0, length 215 bytes (+ int3 padding).
// Callees: own virtual slot 5 (+0x14, IsStillValid), the CurrentUpdateFunc member-function pointer at +0x78 (fn, this
// adjustment at +0x7c), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x4F = CTCCreatureOpinionOfHero,
// bit 15 of the dword at +0x28) and @ 0x004365B0 (type 0xF = CTCTalk, bit 15 of +0x20), CTCTalk::IsTalking
// @ 0x006E28A0, CAIStateGroupBase::IsStopping @ 0x008FD010, the brain's virtual slot 2 (+0x08, yield), the finish call
// @ 0x008FCF40.
// Shape: the PMF uses the multiple-inheritance representation (`#pragma pointers_to_members(full_generality, multiple_inheritance)`: fn +
// adjustment, `mov ecx,[edi+7Ch] ; add ecx,edi`; the PDB slot is 0x10 bytes); both lookups read the never-written out slot on a miss; the rotated yield loop.
// Engine header names; ThingReactingTo is the unaligned CIntelligentPointer at +0x6b (PDB, packed).
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

struct CTCCreatureOpinionOfHero {
    unsigned char _pad[0x340];
    bool ReactionHandled;                       // +0x340
};
struct CTCTalk {
    bool IsTalking();                           // 0x006E28A0
};

struct CAIBrainV {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
};

#pragma pointers_to_members(full_generality, multiple_inheritance)

#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointerCtor ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x73[0x5];               // +0x73
    bool (CAIStateGroup_OpinionReactionRoot::*CurrentUpdateFunc)();    // +0x78 {fn, this adjustment}
    unsigned char _pmf_rest[0x8];               // +0x80 (the PDB member pointer is 0x10 bytes)
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    bool IsStopping();                          // 0x008FD010 (CAIStateGroupBase)
    void FinishUpdate();                        // 0x008FCF40 (CAIStateGroupBase)
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_OpinionReactionRoot::Update()
{
    if (!IsStillValid()) {
        FinishUpdate();
        return;
    }
    if (CurrentUpdateFunc != 0) {
        if (!(this->*CurrentUpdateFunc)()) {
            FinishUpdate();
            return;
        }
        CTCCreatureOpinionOfHero* opinion;
        PBrain->PCreature->GetInterface(&opinion, 0x4f);
        opinion->ReactionHandled = true;
    }
    CTCTalk* talk;
    PBrain->PCreature->GetInterface(&talk, 0xf);
    while (talk->IsTalking()) {
        if (IsStopping())
            break;
        ((CAIBrainV*)PBrain)->Yield();
    }
    FinishUpdate();
}