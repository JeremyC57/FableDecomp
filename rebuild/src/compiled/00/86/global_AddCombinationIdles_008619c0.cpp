// AddCombinationIdles(const CThingCreatureBase& creature, CAnimateThing& animate) -- when the animate thing has no combination animation of set type 25 yet and the creature has an "ST_IDLE_COMB" animation in any group, add a random loaded one as a non-looping combination animation
// Leaf: ?AddCombinationIdles@?A0x
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008619C0, length 187 bytes (+5 int3). `__fastcall` free function in an anonymous
//   namespace (creature in ecx, animate thing in edx), plain `ret`. The leaf is a prefix: the anonymous-namespace hash is
//   per TU (retail ?A0x9eb7a2f3).
// Mangled (Ego_r.pdb): ?AddCombinationIdles@?A0x9eb7a2f3@@YIXABVCThingCreatureBase@@AAVCAnimateThing@@@Z.
// Callees: CAnimateThing::GetFirstCombinationAnimFromType(long) 0x0070C4E0, CCharString(const char*, long) 0x0099EBF0
//   ("ST_IDLE_COMB" @ 0x01276D78), CThingCreatureBase::HasAnim(const CCharString&, const CAnimFilterBase&) const 0x00662C70,
//   CThingCreatureBase::GetRandomLoadedAnimFromName(const CCharString&, bool) const 0x00662FA0 (both landed),
//   CAnimationEntry::GetAnimateThingFlags 0x005DC390, CAnimInit::Init 0x0070C050 (manifest "CInitRumble::CInitRumble"; the
//   7 arguments match Ego_r.pdb `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z`), CAnimationEntry::GetDelayTime
//   0x005DC1F0 (it reads the component of 0x005E0300, between the transition-in time 0x005DC1D0 and flags 0x005DC210
//   getters as in Ego_r.pdb; manifest "CCombatActionBase::GetAnimationTransitionInTime" is propagated),
//   CAnimationEntry::GetTimeToBlendIn(const CThing&) const 0x005DC230, CAnimateThing::AddCombinationAnim(const CAnimInit&,
//   long, long, bool, bool) 0x0070D780 (`ret 14h`), ~CCharString 0x0099EAE0.
// Layout: CAnimInit is 0x2C bytes (Ego_r.pdb; Frame = CAnimFrameInfo {Current, Last} at +0x18, zeroed by the inline
//   default constructor); CAnimFilterAnyGroup is a vptr-only filter (vtable 0x0127261C; landed Clone 0x008350D0).
// Shape: the name is a named local constructed after the early return and destroyed at the end; the filter is a
//   temporary of the HasAnim call (its vtable stored after the argument pushes, no destructor store survives); the
//   animate-thing flags are read into a local BEFORE `CAnimInit init;` (calling GetAnimateThingFlags inside the Init
//   argument list puts the frame zeroing and `push 64h` ahead of the call); the AddCombinationAnim arguments are evaluated
//   right to left (the two `false`, GetDelayTime, GetTimeToBlendIn, then &init). The body must sit INSIDE the anonymous
//   namespace: a global-scope definition after an anonymous-namespace declaration emits `?AddCombinationIdles@@YI...`.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAnimFilterBase {
public:
    virtual bool Passes(long group) const;              // slot 0 (descriptive)
    virtual CAnimFilterBase* Clone() const;             // slot 1
    virtual ~CAnimFilterBase() { }                      // slot 2
};
class CAnimFilterAnyGroup : public CAnimFilterBase {    // vtable 0x0127261C
public:
    virtual bool Passes(long group) const;              // 0x00834CC0
    virtual CAnimFilterBase* Clone() const;             // 0x008350D0
};
class CThing { };
class CAnimationEntry {
public:
    long BankIndex;                                     // +0x00
    long GetDelayTime() const;                          // 0x005DC1F0
    long GetTimeToBlendIn(const CThing& thing) const;   // 0x005DC230
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
};
class CThingCreatureBase : public CThing {
public:
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;                // 0x00662C70
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool exclude) const;   // 0x00662FA0
};
enum EAnimUpdateMode { ANIM_UPDATE_MODE_0, ANIM_UPDATE_MODE_1 };
class CAnimFrameInfo {
public:
    float Current;                                      // +0x00
    float Last;                                         // +0x04
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }
};
class CAnimInit {                                       // Ego_r.pdb, 0x2C bytes
public:
    EAnimUpdateMode UpdateMode;                         // +0x00
    unsigned char   Type;                               // +0x04
    bool            Looping;                            // +0x05
    long            Anim;                               // +0x08
    float           Speed;                              // +0x0C
    float           Alpha;                              // +0x10
    float           LastAlpha;                          // +0x14
    CAnimFrameInfo  Frame;                              // +0x18
    unsigned long   Flags;                              // +0x20
    unsigned long   TypeFlags;                          // +0x24
    long            Priority;                           // +0x28
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping,
              unsigned long flags, long priority);      // 0x0070C050
};
class CAnimSet;
class CAnimateThing {
public:
    CAnimSet* GetFirstCombinationAnimFromType(long type);                                   // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long blendInTime, long delayTime, bool a, bool b);   // 0x0070D780
};

namespace {

void __fastcall AddCombinationIdles(const CThingCreatureBase& creature, CAnimateThing& animate)
{
    if (animate.GetFirstCombinationAnimFromType(0x19))
        return;
    CCharString name("ST_IDLE_COMB");
    if (creature.HasAnim(name, CAnimFilterAnyGroup())) {
        const CAnimationEntry* anim = creature.GetRandomLoadedAnimFromName(name, false);
        if (anim) {
            unsigned long flags = anim->GetAnimateThingFlags();
            CAnimInit init;
            init.Init(0x19, 0, anim->BankIndex, ANIM_UPDATE_MODE_1, false, flags, 100);
            animate.AddCombinationAnim(init, anim->GetTimeToBlendIn(creature), anim->GetDelayTime(), false, false);
        }
    }
}

}