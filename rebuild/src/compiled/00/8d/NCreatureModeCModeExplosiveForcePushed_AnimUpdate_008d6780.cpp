// NCreatureMode::CModeExplosiveForcePushed::AnimUpdate(CAnimateThing&) -- unless the current main animation is type 1 still running, or the force-pushed type 0x1d, start "ST_IDLE_SUBTLE" through the mode's animation filter as the main animation (type 0x1d, looping, priority 100)
// Retry 10: RELOCATION_MATCH -- GetAnimFilter() takes no argument and GetAnimFromName(name, filter, 0) has a third long; the CAnimInit is a temporary passed straight to SetMainAnim (a named local colours BankIndex into esi).
// Leaf: AnimUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6780 (`ret 4`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, the mode's virtual filter getter (slot 4,
// `call [eax+10h]`), CThingCreatureBase::GetAnimFromName(const CCharString&, const CAnimFilterBase*, long) @ 0x00662B10,
// CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim @ 0x0070D580.
// Layout: CAnimSet (retail, 12-byte vectors): Type +0x50, Looping/Finished bits at +0x51. CAnimInit (Ego_r.pdb, 0x2c).
// Shape: the skip test is a ternary over the type (`cmp eax,1 ; jne ; test [51h],2 ; jmp ; cmp eax,1Dh ; je`); the
// animation name is a named local in its own block (built before the filter call); the parent is read first.
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
    unsigned char  m_stateBits;            // +0x51 Looping (bit 0), Finished (bit 1), Forwards, HadUpdate, OwnedByMainAnim
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay);   // 0x0070D580
};

struct CThingCreatureBase {
    CAnimationEntry* GetAnimFromName(const CCharString& name, long flags);                    // 0x00662A00
    CAnimationEntry* GetAnimFromName(const CCharString& name, const CAnimFilterBase* filter, long flags); // 0x00662B10
};

struct CModeBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual const CAnimFilterBase* GetAnimFilter();   // slot 4 (no argument)
    CThingCreatureBase* ParentThing;       // +0x04
};

namespace NCreatureMode {

struct CModeExplosiveForcePushed : CModeBase {
    void AnimUpdate(CAnimateThing& anim);
};

void CModeExplosiveForcePushed::AnimUpdate(CAnimateThing& anim)
{
    long type = anim.Type;
    if (type == 1) {
        if (!(anim.m_stateBits & 2))
            return;
    }
    else if (type == 0x1d)
        return;
    CThingCreatureBase* parent = ParentThing;
    CAnimationEntry* entry;
    {
        CCharString name("ST_IDLE_SUBTLE");
        entry = parent->GetAnimFromName(name, GetAnimFilter(), 0);
    }
    anim.SetMainAnim(CAnimInit(0x1d, 0, entry->BankIndex, 1, true, 0, 100), 0, 0);
}

}