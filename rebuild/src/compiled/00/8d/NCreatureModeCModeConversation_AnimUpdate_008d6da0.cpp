// NCreatureMode::CModeConversation::AnimUpdate(CAnimateThing&) -- while the creature is idle (no action), keep a conversation idle playing: unless the idle animation (type 0x1c) is already looping and no reset was asked for, start a random loaded "ST_IDLE" animation (blend-in at least 7 frames when switching from another type)
// Leaf: AnimUpdate
// Retry 10: RELOCATION_MATCH -- `CThing* thing = ParentThing;` local for GetTimeToBlendIn plus the CAnimInit as a temporary argument of SetMainAnim.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6DA0, 220 bytes (`ret 4`).
// Callees: CThingCreatureBase::IsPerformingAnyAction 0x006622A0, GetRandomLoadedAnimFromName(const CCharString&, bool) 0x00662FA0;
// CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CAnimationEntry::GetTimeToBlendIn(CThing*) 0x005DC230,
// GetAnimateThingFlags 0x005DC390, GetAnimationTransitionInTime 0x005DC1F0; CAnimInit::Set 0x0070C050 (manifest "CInitRumble");
// CAnimateThing::SetMainAnim(const CAnimInit&, long, long) 0x0070D580.
// Layout (PDB CModeConversation 0x34, matches retail): CModeBase 0x1c (ParentThing +0x04), CurrentAttitude +0x1c,
// InitialAttitude +0x20, Filter +0x24, ResetIdleAnim +0x30. CAnimateThing: anim vector begin/end +0x04/+0x08, current anim
// type +0x50, flags +0x51 (bit 0 / bit 1).
// Shape: the sibling of CModeBlock::AnimUpdate 0x008D8DD0; the animation name is a named CCharString local in its own block
// (`lea ecx ; push ecx`, built in the dead argument slot and destroyed right after the lookup); the blend-in time is a
// local clamped before the CAnimInit record is built.
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

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
};

enum EConversationAttitude { CONVERSATION_ATTITUDE_NEUTRAL = 0 };

namespace NCreatureMode {

struct CModeConversation : CModeBase {
    EConversationAttitude CurrentAttitude; // +0x1c
    EConversationAttitude InitialAttitude; // +0x20
    CAnimFilterOfGroup    Filter;          // +0x24
    bool                  ResetIdleAnim;   // +0x30
    void AnimUpdate(CAnimateThing& animate);
};

void CModeConversation::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = ParentThing;
    if (creature->IsPerformingAnyAction())
        return;
    if (animate.AnimsBegin != animate.AnimsEnd && !(animate.CurrentAnimFlags & 2) && animate.CurrentAnimType == 0x1c
        && !(animate.CurrentAnimFlags & 1) && !ResetIdleAnim)
        return;
    CAnimationEntry* entry;
    {
        CCharString name("ST_IDLE");
        entry = creature->GetRandomLoadedAnimFromName(name, false);
    }
    if (entry) {
        CThing* thing = ParentThing;
        long blendIn = entry->GetTimeToBlendIn(thing);
        if (animate.CurrentAnimType != 0x1c && blendIn < 7)
            blendIn = 7;
        animate.SetMainAnim(CAnimInit(0x1c, 0, entry->AnimIndex, 1, false, entry->GetAnimateThingFlags(), 100), blendIn, entry->GetAnimationTransitionInTime());
    }
    ResetIdleAnim = false;
}

}