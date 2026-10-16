// Grade: DIFFER(219v224) -- the message-sender read is an inlined accessor leaving a dead frame store + constant-false `xor cl,cl ; jne` (same shape as 0x00744D00); also retail `dec dword [eax+0D0h]` in place and esi/edi swapped | tried: plain sender = GetPItem() form | next: recover the inlined CMessageEvent sender accessor (shared with 0x00744D00)
// CAIStateGroup_MinionReactToCrime::OnReplace() -- release the forced-open building, withdraw our own "getting help"
// message if the guard was not told, and clear the opinion deed mask if this state group applied it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958B20, length 215 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, GetSomeoneElseGettingHelpMessage @ 0x00958770,
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (+0x60 = CMessageEventManager), CMessageEventManager::RemoveMessage
// @ 0x00493B5B, CVectorMap::LowerBound @ 0x0040F020 (inlined lookup, type 0x4F = CTCCreatureOpinionOfHero, bit 15 of
// the dword at +0x28), the opinion component's SetOpinionDeedMask @ 0x006821E0 (thiscall, arg = [[comp+1ch]+38h]).
// Shape: IS_THING_ALIVE(ForceOpenBuilding) then `--building->+0xd0`; the message sender read is an inlined accessor.
// Engine header names (OpinionDeedHandle +0x14, GuardTold +0x18, DeedMaskApplied +0x1a, ForceOpenBuilding +0x1b).
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

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CThingBuildingUse {
    unsigned char _pad[0xd0];
    long ForcedOpenCount;                       // +0xd0
};

struct CMessageEvent {
    unsigned char _pad[0x48];
    CBaseIntelligentPointerCtor PSender;        // +0x48
};
struct CMessageEventManager {
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
};

struct COpinionDeedMaskSource { unsigned char _pad[0x38]; long Mask; };
struct CTCCreatureOpinionOfHero {
    unsigned char _pad[0x1c];
    COpinionDeedMaskSource* PDef;               // +0x1c
    void SetOpinionDeedMask(long mask);         // 0x006821E0
};

#pragma pack(push, 1)
struct CAIStateGroup_MinionReactToCrime : CAIStateGroupBase {
    long OpinionDeedHandle;                     // +0x14
    bool GuardTold;                             // +0x18
    bool AllowDuplicateReactToCrime;            // +0x19
    bool DeedMaskApplied;                       // +0x1a
    CBaseIntelligentPointerCtor ForceOpenBuilding;  // +0x1b CIntelligentPointer<CThingBuilding> (unaligned, PDB)
    CMessageEvent* GetSomeoneElseGettingHelpMessage(long handle);   // 0x00958770
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    virtual void OnReplace();
};
#pragma pack(pop)

void CAIStateGroup_MinionReactToCrime::OnReplace()
{
    if (IS_THING_ALIVE(ForceOpenBuilding))
        --((CThingBuildingUse*)ForceOpenBuilding.GetPItem())->ForcedOpenCount;
    if (!GuardTold) {
        CMessageEvent* message = GetSomeoneElseGettingHelpMessage(OpinionDeedHandle);
        if (message != 0) {
            CThing* sender = 0;
            if (message->PSender.GetPItem() != 0)
                sender = message->PSender.GetPItem();
            if (sender == PBrain->PCreature)
                GetPWorld()->PMessageEventManager->RemoveMessage(message);
        }
    }
    GuardTold = false;
    if (DeedMaskApplied) {
        if (PBrain->PCreature->HasTC(0x4f)) {
            CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)PBrain->PCreature->FindEntry(0x4f)->component;
            opinion->SetOpinionDeedMask(opinion->PDef->Mask);
        }
        DeedMaskApplied = false;
    }
}
