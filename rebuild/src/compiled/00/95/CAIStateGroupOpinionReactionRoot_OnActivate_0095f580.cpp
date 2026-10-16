// CAIStateGroup_OpinionReactionRoot::OnActivate() -- cut short the creature's current action when more than the
// allowed seconds of it remain (and it is not a looping one); log the current reaction against the thing reacted to
// and take that reaction's "allow speech on non-pure-AI speaker" flag; cache the creature's CTCTalk.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F580, length 213 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetFramesRemaining @ 0x006925D0 (on the creature's current action at +0x100),
// CThingCreatureBase::FinishCurrentAction @ 0x00662650, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (+0x74 = COpinionReactionManager), COpinionReactionManager::LogReaction
// @ 0x00700940, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface into PTCTalk, type 0xF = CTCTalk, bit 15 of
// the dword at +0x20); frames-per-second long @ 0x01375550, the seconds float global @ 0x0122DEDC.
// Shape: the frame rate is read into a local before the GetFramesRemaining call (both fild from the frame), the
// reaction manager pointer is a named local; the per-reaction flag table is a byte array at manager+0x45f.
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

extern long g_framesPerSecond;                  // 0x01375550
extern float g_maxActionSecondsBeforeReaction;  // 0x0122DEDC

struct CCreatureActionBase {
    unsigned char _pad[0x20];
    long LoopCount;                             // +0x20
    long GetFramesRemaining();                  // 0x006925D0
};
struct CThingCreatureBase {
    unsigned char _pad[0x100];
    CCreatureActionBase* PCurrentAction;        // +0x100
    void FinishCurrentAction();                 // 0x00662650
};

struct COpinionReactionManager {
    unsigned char _pad[0x45f];
    bool AllowSpeechOnNonPureAISpeaker[1];      // +0x45f, indexed by reaction type
    void LogReaction(long reactionType, CThing* creature, bool flag);   // 0x00700940
};
struct CWorld {
    unsigned char _pad[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
};

struct CTCTalk;

#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointerCtor ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x73[0x5];               // +0x73
    unsigned char CurrentUpdateFunc[0x10];      // +0x78
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    virtual void OnActivate();
};
#pragma pack(pop)

void CAIStateGroup_OpinionReactionRoot::OnActivate()
{
    CCreatureActionBase* action = ((CThingCreatureBase*)PBrain->PCreature)->PCurrentAction;
    if (action != 0) {
        long framesPerSecond = g_framesPerSecond;
        if (action->GetFramesRemaining() > framesPerSecond * g_maxActionSecondsBeforeReaction && action->LoopCount <= 0)
            ((CThingCreatureBase*)PBrain->PCreature)->FinishCurrentAction();
    }
    AllowSpeechOnNonPureAISpeaker = false;
    if (CurrentReactionType != 0 && ThingReactingTo.GetPItem() != 0) {
        COpinionReactionManager* manager = GetPWorld()->POpinionReactionManager;
        manager->LogReaction(CurrentReactionType, PBrain->PCreature, true);
        AllowSpeechOnNonPureAISpeaker = manager->AllowSpeechOnNonPureAISpeaker[CurrentReactionType];
    }
    PBrain->PCreature->GetInterface(&PTCTalk, 0xf);
}