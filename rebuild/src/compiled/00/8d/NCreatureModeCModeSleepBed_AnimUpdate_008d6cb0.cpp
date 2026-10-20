// NCreatureMode::CModeSleepBed::AnimUpdate(CAnimateThing&) -- unless the sleep animation (type 0x16) is playing and unfinished, start SleepAnimName as the main animation (type 0x16, looping, priority 100) with the entry's blend-in time
// Retry 10: RELOCATION_MATCH -- the CAnimInit is a temporary passed straight to SetMainAnim (a named local colours BankIndex into a callee-saved register and hoists the load).
// Leaf: AnimUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6CB0 (`ret 4`).
// Callees: CThingCreatureBase::GetAnimFromName(const CCharString&, long) @ 0x00662A00, CAnimationEntry::GetAnimateThingFlags
// @ 0x005DC390 / GetTimeToBlendIn @ 0x005DC230, CAnimInit::Set (the 7-argument initialiser) @ 0x0070C050,
// CAnimateThing::SetMainAnim @ 0x0070D580.
// Layout: CAnimSet (retail, 12-byte vectors): Anims first/last +4/+8, Type +0x50, Looping/Finished bits at +0x51.
// CAnimInit (Ego_r.pdb, 0x2c) is a frame local; its Frame member is zeroed by the inline constructor (the two
// `mov [esp+..],0` stores right before the Set call).
// CModeSleepBed layout as the landed constructor 0x0084B7C0: SleepAnimName +0x28.
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CAnimFilterBase;

struct CAnimationEntry {                   // Ego_r.pdb CAnimationEntry (0x10)
    long BankIndex;                        // +0
    unsigned char _rest[0xc];              // AnimName, GroupName, Components
    unsigned long GetAnimateThingFlags() const;        // 0x005DC390
    long GetTimeToBlendIn(CThing* thing) const;        // 0x005DC230
};

struct CAnimFrameInfo {                    // 8 bytes, zeroed by its inline constructor
    long m_frame;
    long m_subFrame;
    CAnimFrameInfo() : m_frame(0), m_subFrame(0) { }
};

struct CAnimInit {                         // Ego_r.pdb CAnimInit (0x2c)
    long           UpdateMode;             // +0x00
    unsigned char  Type;                   // +0x04
    bool           Looping;                // +0x05
    long           Anim;                   // +0x08
    float          Speed;                  // +0x0c
    float          Alpha;                  // +0x10
    float          LastAlpha;              // +0x14
    CAnimFrameInfo Frame;                  // +0x18
    unsigned long  Flags;                  // +0x20
    unsigned long  TypeFlags;              // +0x24
    long           Priority;               // +0x28
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority)
    {
        Set(type, typeFlags, anim, updateMode, looping, flags, priority);
    }
    void Set(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050
};

struct CAnimateThing {                     // starts with CCompleteAnimation::MainAnimSet (retail CAnimSet, 12-byte vectors)
    long           UpdateMode;             // +0x00
    short*         m_animsFirst;           // +0x04 Anims (vector<short>)
    short*         m_animsLast;            // +0x08
    unsigned char  _pad0c[0x44];
    unsigned char  Type;                   // +0x50
    unsigned char  Looping : 1;            // +0x51 bit 0
    unsigned char  Finished : 1;           // +0x51 bit 1
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay);   // 0x0070D580
};

struct CThingCreatureBase {
    CAnimationEntry* GetAnimFromName(const CCharString& name, long flags);                    // 0x00662A00
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase* filter); // 0x00662B10
};

struct CModeBase {
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char _body[0x14];
    CThingCreatureBase* GetParent() const { return ParentThing; }
};

namespace NCreatureMode {

struct CModeSleepBed : CModeBase {
    unsigned char _filter[0xc];            // +0x1c CAnimFilterOfGroup Filter
    CCharString   SleepAnimName;           // +0x28
    void AnimUpdate(CAnimateThing& anim);
};

void CModeSleepBed::AnimUpdate(CAnimateThing& anim)
{
    if (anim.m_animsFirst == anim.m_animsLast || anim.Finished || anim.Type != 0x16) {
        CAnimationEntry* entry = GetParent()->GetAnimFromName(SleepAnimName, 0L);
        CThing* thing = (CThing*)ParentThing;
        anim.SetMainAnim(CAnimInit(0x16, 0, entry->BankIndex, 1, true, entry->GetAnimateThingFlags(), 100), entry->GetTimeToBlendIn(thing), 0);
    }
}

}