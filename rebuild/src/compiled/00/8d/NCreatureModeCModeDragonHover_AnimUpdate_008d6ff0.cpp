// NCreatureMode::CModeDragonHover::AnimUpdate(CAnimateThing&) -- while the dragon is idle (no action) and its animation list is empty or the current one may be replaced, start a random loaded "ST_IDLE_SUBTLE" animation
// Leaf: ?AnimUpdate@CModeDragonHover
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6FF0, 176 bytes, `ret 4`; vtable 0x0126845C slot 2.
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, CCharString(const char*, long) @ 0x0099EBF0
//   ("ST_IDLE_SUBTLE" @ 0x01276DBC), CThingCreatureBase::GetRandomLoadedAnimFromName(const CCharString&, bool) @ 0x00662FA0,
//   ~CCharString @ 0x0099EAE0, CAnimationEntry::GetAnimateThingFlags @ 0x005DC390, CAnimInit::Set @ 0x0070C050,
//   CAnimationEntry::GetAnimationTransitionInTime @ 0x005DC1F0, CAnimationEntry::GetTimeToBlendIn(CThing*) @ 0x005DC230,
//   CAnimateThing::SetMainAnim(const CAnimInit&, long, long) @ 0x0070D580.
// Name: FableWin.pdb `?AnimUpdate@CModeDragonHover@NCreatureMode@@UAEXAAVCAnimateThing@@@Z` (slot 2).
// Shape: the landed sibling CModeConversation::AnimUpdate @ 0x008D6DA0 (same CAnimInit / CAnimateThing / CAnimationEntry
//   models): the name is a named CCharString local in its own block; here the blend-in time is passed straight as an
//   argument, so the CAnimInit temporary (type 6, flag05 true) is built first and the two entry getters follow right to left.
struct CThing {
    unsigned char _pad0[0x50];
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

struct CAnimationEntry;

struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();          // 0x006622A0
    CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loadedOnly);   // 0x00662FA0
};

struct CBlendPair {                        // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};

struct CAnimInit {                         // 0x2c bytes
    long          Mode;                    // +0x00
    unsigned char AnimType;                // +0x04
    bool          Flag05;                  // +0x05
    long          AnimIndex;               // +0x08
    float         Scales[3];               // +0x0c
    CBlendPair    Blend;                   // +0x18
    unsigned long Flags;                   // +0x20
    long          Arg24;                   // +0x24
    long          Percent;                 // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

struct CAnimationEntry {
    long AnimIndex;                        // +0x00
    unsigned long GetAnimateThingFlags();  // 0x005DC390
    long GetAnimationTransitionInTime();   // 0x005DC1F0
    long GetTimeToBlendIn(CThing* thing);  // 0x005DC230
};

struct CAnimateThing {
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition);   // 0x0070D580
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
};

namespace NCreatureMode {
struct CModeDragonHover : CModeBase {      // vtable 0x0126845C
    void AnimUpdate(CAnimateThing& animate);
};
}

void NCreatureMode::CModeDragonHover::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = ParentThing;
    if (creature->IsPerformingAnyAction())
        return;
    if (animate.AnimsBegin != animate.AnimsEnd && !(animate.CurrentAnimFlags & 2))
        return;
    CAnimationEntry* entry;
    {
        CCharString name("ST_IDLE_SUBTLE");
        entry = creature->GetRandomLoadedAnimFromName(name, false);
    }
    if (entry) {
        CThing* thing = ParentThing;
        animate.SetMainAnim(CAnimInit(6, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100),
            entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
    }
}