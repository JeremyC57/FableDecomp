// NCreatureMode::CModeSitBase::AnimUpdate(CAnimateThing&) -- while no remove-action is pending, count the idle timer down; when the sitting animation (type 0x15) is playing, start a one-off ST_IDLE action once the timer runs out, otherwise (re)start ST_IDLE_SUBTLE and, after an idle, re-seed the timer with a random frame count
// Leaf: ?AnimUpdate@CModeSitBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6AC0, 444 bytes (padded to 448), `ret 4`; CModeSitChair vtable 0x01267B74 slot 2
//   (inherited; also CModeSleepBed's sibling slot).
// Name: FableWin.pdb `?AnimUpdate@CModeSitBase@NCreatureMode@@UAEXAAVCAnimateThing@@@Z` -- CModeSitChair has no AnimUpdate of
//   its own, so the catalog's CModeSitChair::vfunc_2 is the base (as the landed CModeSitBase::OnActivate 0x008D6580, slot 5).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("ST_IDLE" 0x0127138C, "ST_IDLE_SUBTLE" 0x01276DBC) / ~CCharString
//   @ 0x0099EAE0, CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, const CSoundPair&, bool, bool, bool,
//   long, long) @ 0x008424F0, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0,
//   ~CCreatureAction_PlayAnimation @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated),
//   CThingCreatureBase::GetAnimFromName @ 0x00662A00, CAnimationEntry::GetAnimateThingFlags @ 0x005DC390 / GetTimeToBlendIn
//   @ 0x005DC230, CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim @ 0x0070D580, virtual GetType (slot 1),
//   CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims @ 0x0073A6F0, _ftol @ 0x00BFEA70, CModeBase::GetWorld @ 0x0073A680,
//   the world's random seed accessor @ 0x0049D810, GFROR13 @ 0x00497890.
// Globals: 0.75f @ 0x01238174, -1.5f @ 0x01285670.
// Layout: Ego_r.pdb CModeSitBase (0x2c): IdleCounter +0x1c, PlayingIdle +0x20, PRemoveAction +0x24 (CCountedPointer). CAnimInit /
//   CAnimateThing as the landed CModeSleepBed::AnimUpdate 0x008D6CB0; CSoundPair as the landed PlayAnimation ctor.
// Shape: the creature is a named local read after the counter decrement; both names are named locals in their own scopes
//   (a bare temporary pushes the constructor's returned `this`; retail re-derives `lea`); the action is a temporary of the
//   SetCurrentAction call whose result is kept across the name's destructor; the re-seed is the landed CModeSitBase::OnActivate
//   body; `PlayingIdle = false` is unconditional (retail's skip jumps onto the store). CAnimFrameInfo is two floats (Ego_r.pdb):
//   with longs VC shares one zero register with the TypeFlags push. 4 grades.
class CThing;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair { public:                        // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CAnimationEntry { public:                   // Ego_r.pdb CAnimationEntry (0x10)
    long BankIndex;                        // +0
    unsigned char _rest[0xc];
    unsigned long GetAnimateThingFlags() const;        // 0x005DC390
    long GetTimeToBlendIn(CThing* thing) const;        // 0x005DC230
};
class CAnimFrameInfo { public:                    // Ego_r.pdb CAnimFrameInfo (8 bytes), zeroed by its inline constructor
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0.0f), Last(0.0f) { }   // float zeros: immediate stores, not the integer zero register
};
class CAnimInit { public:                         // Ego_r.pdb CAnimInit (0x2c)
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
class CAnimateThing { public:                     // starts with CCompleteAnimation::MainAnimSet (retail CAnimSet, 12-byte vectors)
    long           UpdateMode;             // +0x00
    short*         m_animsFirst;           // +0x04 Anims (vector<short>)
    short*         m_animsLast;            // +0x08
    unsigned char  _pad0c[0x44];
    unsigned char  Type;                   // +0x50
    unsigned char  Looping : 1;            // +0x51 bit 0
    unsigned char  Finished : 1;           // +0x51 bit 1
    void SetMainAnim(const CAnimInit& init, long blendIn, long delay);   // 0x0070D580
};

class CCreatureActionBase { public:               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
};
class CThingCreatureBase;
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:   // vtable 0x01273A0C, retail 0xb4
    unsigned char _own[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animName, const CSoundPair& sound,
        bool useAnimationMovement, bool usePhysics, bool stayOnLastFrame, long numberOfTimesToLoop, long delay);   // 0x008424F0
    ~CCreatureAction_PlayAnimation();      // 0x00693EF0
};
class CThingCreatureBase { public:
    CAnimationEntry* GetAnimFromName(const CCharString& name, long flags);   // 0x00662A00
    bool SetCurrentAction(const CCreatureActionBase& action);               // 0x006644F0
};

struct CRandomSeed {
    unsigned long Seed;
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CWorld { public:
    CRandomSeed* GetRandomSeed();          // 0x0049D810
};
inline unsigned long GFRandom(CRandomSeed* random)
{
    random->Seed = random->Seed * 0x24a1 + 0x24df;
    random->Seed = GFROR13(random->Seed);
    return random->Seed;
}

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    virtual unsigned long GetType() const; // slot 1
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char _body[0x14];
    CWorld* GetWorld() const;              // 0x0073A680
    long GetNoFramesBeforePlayingCombinationIdleAnims(unsigned long mode);   // 0x0073A6F0
};

namespace NCreatureMode {
class CModeSitBase : public CModeBase { public:
    long          IdleCounter;             // +0x1c
    bool          PlayingIdle;             // +0x20
    CCreatureActionBase* PRemoveAction;    // +0x24 CCountedPointer<CCreatureActionBase>::Data
    void*         PRemoveActionInfo;       // +0x28
    void AnimUpdate(CAnimateThing& anim);
};

void CModeSitBase::AnimUpdate(CAnimateThing& anim)
{
    if (PRemoveAction == 0) {
        --IdleCounter;
        CThingCreatureBase* creature = ParentThing;
        if (anim.m_animsFirst != anim.m_animsLast && !anim.Finished && anim.Type == 0x15) {
            if (!PlayingIdle && IdleCounter < 0) {
                bool started;
                {
                    CCharString name("ST_IDLE");
                    started = creature->SetCurrentAction(CCreatureAction_PlayAnimation(*creature, name, CSoundPair(),
                        false, true, false, 0, 0));
                }
                if (started)
                    PlayingIdle = true;
            }
        }
        else {
            CAnimationEntry* entry;
            {
                CCharString name("ST_IDLE_SUBTLE");
                entry = creature->GetAnimFromName(name, 0L);
            }
            CThing* thing = (CThing*)ParentThing;
            anim.SetMainAnim(CAnimInit(0x15, 0, entry->BankIndex, 1, true, entry->GetAnimateThingFlags(), 100),
                entry->GetTimeToBlendIn(thing), 0);
            if (PlayingIdle) {
                float frames = (float)GetNoFramesBeforePlayingCombinationIdleAnims(GetType());
                long minFrames = (long)(frames * 0.75f);
                CRandomSeed* random = GetWorld()->GetRandomSeed();
                unsigned long range = -minFrames - (long)(frames * -1.5f);
                unsigned long r = GFRandom(random);
                IdleCounter = (range ? r % range : 0) + minFrames;
            }
            PlayingIdle = false;
        }
    }
}
}