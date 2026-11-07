// CAIStateGroup_DeedReactionRoot::Update() -- react to a hero deed: pick the attitude-indexed reaction animation and dialogue tag from the deed log, wait for higher-priority actions, look at the deed's thing, play the reaction speech (logging it with the opinion-reaction manager) and the reaction animation, then wait for the talk to finish
// Grade: DIFFER(1580v1600) -- chunk 826 (18 attempts, best c17, md 135 from 438): fixed the CThingCreatureBase layout (pad from 0x94: PCurrentAction +0x100, the parked source read +0x104), the CDefPointer temporaries released inline (forced-inline Release holding the pointee in a local, engine/CDefPointer.h's shape), animation->IsEmpty() tested in place (new opt-in FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY), the creature reference hoisted before the attitude if (source stays in esi, GetCreature loads in the test block as retail), standalone HasTC as the if-return form (`test ch,ch ; jns` as retail) with PeekTC/GetTC's presence written in place; residue = the bit-16 presence load narrowed to `mov cl,[eax+2Ah]` (retail `mov ecx,[eax+28h] ; shr ecx,10h`), frame slots +4 (key home, attitude), the GetSex operand order, the while-loop 0x32 kept in esi, and the AIAction_LookAtThing CIntelligentPointer argument (retail inlines the copy body here and calls 0x00654D58 at the PerformAction site: a per-site budget split)
// Next idea: the bit-16 narrowing (Hawking 0x0094BB20 keeps shr with a plain bool live across LowerBound; here only the `const bool&` form keeps the dead spill and it narrows); then the copy-ctor per-site split (/d2inlS40 inlines more: function-budget side)
// Leaf: ?Update@CAIStateGroup_DeedReactionRoot@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00960120, 1580 bytes + a 5-entry switch table at 0x0096074C, plain `ret` (virtual; Ego_r.pdb
//   ?Update@CAIStateGroup_DeedReactionRoot@@UAEXXZ, CAIStateGroupBase vtable slot 12 Update).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90, CIntelligentPointer<const CThing>
//   copy 0x00654D58 (out of line), CAIStateGroupBase::GetWorld 0x008FCFC0 / Terminate 0x008FCF40 / IsStopping 0x008FD010 /
//   AIAction_LookAtThing 0x0097D6F0 / AIAction_PerformActionTargetFaceAndWait 0x0097DCB0 / AIAction_DoNothing 0x0097E200,
//   CVectorMap LowerBound 0x0040F020 / const 0x004365B0, COpinionReactionManager::GetAttitudeOrReactionType 0x00701010 /
//   GetThingAttitudeToPlayer 0x00701840 / GetBlurredAttitude 0x00700120 / LogDeedReactionPlayingSpeech 0x006FEAE0,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630, CThingCreatureBase::GetSex 0x00661FF0 / GetCreatureType 0x00662000,
//   CTCWife::IsMarriedToThePlayer 0x006CEC80 / IsCourtableAndNotBlockingIt 0x006CF810,
//   CTCCreatureOpinionOfHero::GetGreetDialogueGroupIndex 0x00683C10, CTCTalk::PlayAISpeech 0x006E3B60 / IsTalking 0x006E28A0,
//   CCharString::operator!=(const char*) 0x0099E960, CBasicString<char>::operator==(const char*) 0x004115A0,
//   IS_THING_DEAD<CIntelligentPointer<const CThing> > 0x00836BE0, CCreatureAction_PlayAnimation ctor 0x008424F0,
//   ~CCreatureActionBase 0x00693EF0.
// Layout: Ego_r.pdb CAIStateGroup_DeedReactionRoot (ThingReactingTo +0x14 .. DeliveredSpeech +0x35); CWorld POpinionReactionManager
//   +0x74; COpinionReactionManager AttitudeMask +0x554 / DeedReactionSpeechPlaying +0x5A4 (Ego_r less the 8 bytes of the
//   Reactions list); CTCHeroOpinionDeedLog DeedReactions +0xE8 (COpinionDeedReaction 0x90: Animations[18], DialogueTags[18]
//   +0x48); COpinionDeedEffectsDef IsFlirting +0x7C bit 6, AllowSpeechOnNonPureAISpeaker +0x7D bit 0, ReactionDialogueLayer +0x60;
//   CThingCreatureBase PCurrentAction +0x100, the action's priority +0x20; retail CThing interface bitset +0x20, map +0x44.
// TC kinds: 0x50 CTCOpinionSource, 0x51 CTCHeroOpinionDeedLog, 0xD7 CTCWife, 0x4F CTCCreatureOpinionOfHero, 0x0F CTCTalk.
// Game: 0x013B86A0 CMainGameComponent::PMainGameComponent (player manager +0x1C).
class CTCBase;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY
#include "fable_string.h"

class CBaseObject { public:
    virtual ~CBaseObject();
};
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#include "engine/CIntelligentPointer.h"

enum EOpinionDeedType { };
enum EOpinionAttitudeType { };
enum EOpinionReactionType { };
enum ESex { };
enum ECreatureType { };
enum EAIActionReturn { };
enum EAIActionRepeat { };
namespace NSpeechGainManager { enum EDialogueLayer { }; }

class CThing : public CBaseObject { public:
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Flags91;                      // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        return (it == map->end() || type < it->first ? map->end() : it)->second;
    }
    template<class T> __forceinline bool GetTC(T** out, long type)
    {
        bool present = (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0;
        if (present) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            *out = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
        }
        return present != 0;
    }
    template<class T> __forceinline bool PeekTC(const T** out, long type) const
    {
        const bool& present = (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0;
        if (present) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            *out = (const T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
        }
        return present != 0;
    }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0x20 - 0x04];
    long Priority;                              // +0x20
    unsigned char _pad24[0xa8 - 0x24];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool IsInterruptable() const;       // slot 27 (+0x6C)
};
class CSoundPair { public:
    long First;
    long Second;
    CSoundPair() : First(0), Second(0) { }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad94[0x100 - 0x94];
    CCreatureActionBase* PCurrentAction;        // +0x100
    ESex GetSex() const;                        // 0x00661FF0
    ECreatureType GetCreatureType() const;      // 0x00662000
};
class CThingAICreature : public CThingCreatureBase { };
class CThingPlayerCreature : public CThingCreatureBase { };
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
template <class T> bool __fastcall IS_THING_DEAD(const T& thing);     // 0x00836BE0 (folded body)
template <class T> __forceinline bool IS_THING_ALIVE(const T& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.Get()->Flags91 & 1))
            return true;
    }
    return false;
}

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CMainGameComponent { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
    static CMainGameComponent* PMainGameComponent;   // 0x013B86A0
};
class CTCOpinionSource;
class COpinionReactionManager { public:
    unsigned char _pad00[0x554];
    bool AttitudeMask[79];                      // +0x554
    unsigned char _pad5a3;
    long DeedReactionSpeechPlaying[92];         // +0x5a4
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;          // 0x00701840
    EOpinionAttitudeType GetBlurredAttitude(EOpinionAttitudeType attitude) const;     // 0x00700120
    void LogDeedReactionPlayingSpeech(EOpinionDeedType deed, bool playing);          // 0x006FEAE0
private:
    friend class CAIStateGroup_DeedReactionRoot;
    EOpinionReactionType GetAttitudeOrReactionType(const CThing& thing, bool attitude, const bool* mask,
                                                   const CTCOpinionSource* source) const;   // 0x00701010
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74 CCountedPointer<COpinionReactionManager>
    COpinionReactionManager& GetOpinionReactionManager() { return *POpinionReactionManager; }
};
class CDefBase { public:
    virtual void v00();
    virtual void Destroy() const;               // slot 1
    mutable long RefCount;                      // +0x04
};
class COpinionDeedEffectsDef : public CDefBase { public:
    unsigned char _pad08[0x60 - 0x08];
    NSpeechGainManager::EDialogueLayer ReactionDialogueLayer;   // +0x60
    unsigned char _pad64[0x7c - 0x64];
    bool CanBeHeard : 1;                        // +0x7c
    bool ReactionRestrictToTarget : 1;
    bool AllowSimultaneousReactionDialogue : 1;
    bool CanApologise : 1;
    bool MustNotRunWhenWarned : 1;
    bool OnlyGetFinedForOneInstance : 1;
    bool IsFlirting : 1;                        // +0x7c bit 6
    bool OnReactSetGreeted : 1;
    bool AllowSpeechOnNonPureAISpeaker : 1;     // +0x7d bit 0
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { Release(); }
    __forceinline void Release()
    {
        if (Object) {
            T* object = Object;
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
class COpinionDeedReaction { public:
    CCharString Animations[18];                 // +0x00
    unsigned long DialogueTags[18];             // +0x48
};
class CTCHeroOpinionDeedLog { public:
    unsigned char _pad00[0xe8];
    COpinionDeedReaction DeedReactions[92];     // +0xe8
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;   // 0x006C5630
};
class CTCWife { public:
    bool IsMarriedToThePlayer() const;          // 0x006CEC80
    bool IsCourtableAndNotBlockingIt() const;   // 0x006CF810
};
class CTCCreatureOpinionOfHero { public:
    unsigned long GetGreetDialogueGroupIndex(EOpinionAttitudeType attitude);   // 0x00683C10
};
class CTCTalk { public:
    enum ESpeechPlaybackResult { };
    ESpeechPlaybackResult PlayAISpeech(unsigned long tag, bool force, NSpeechGainManager::EDialogueLayer layer, bool a, bool b);   // 0x006E3B60
    bool IsTalking() const;                     // 0x006E28A0
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThingAICreature* Creature;                 // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }
    void Terminate();                           // 0x008FCF40
protected:
    CWorld& GetWorld() const;                   // 0x008FCFC0
    bool IsStopping() const;                    // 0x008FD010
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_PerformActionTargetFaceAndWait(const CCreatureActionBase& action, CIntelligentPointer<const CThing> target,
                                                             bool wait);   // 0x0097DCB0
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
};
class CAIStateGroup_DeedReactionRoot : public CAIStateGroupBase { public:
    CIntelligentPointer<const CThing> ThingReactingTo;  // +0x14
    EOpinionDeedType CurrentDeedType;           // +0x1c
    long CurrentDeedHandle;                     // +0x20
    bool DeedIsVirtual;                         // +0x24
    unsigned char _pad25[3];
    unsigned long LastThingOpinionSourceUID[2]; // +0x28
    long LastOpinionSourceVirtualDeedID;        // +0x30
    bool DeliveredReaction;                     // +0x34
    bool DeliveredSpeech;                       // +0x35
    virtual void Update();
};

void CAIStateGroup_DeedReactionRoot::Update()
{
    if (!IS_THING_ALIVE(ThingReactingTo)) {
        Terminate();
        return;
    }
    COpinionReactionManager& reactions = GetWorld().GetOpinionReactionManager();
    const CTCOpinionSource* source = 0;
    ThingReactingTo.Get()->PeekTC(&source, 0x50);
    const CThingAICreature& creature = GetCreature();
    EOpinionAttitudeType attitude;
    if (source)
        attitude = (EOpinionAttitudeType)reactions.GetAttitudeOrReactionType(creature, true, reactions.AttitudeMask, source);
    else
        attitude = reactions.GetBlurredAttitude(reactions.GetThingAttitudeToPlayer(creature));
    const CThing* deedThing;
    if (DeedIsVirtual) {
        deedThing = CMainGameComponent::PMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (deedThing == 0) {
            DeliveredReaction = true;
            Terminate();
            return;
        }
    } else
        deedThing = ThingReactingTo.Get();
    const CTCHeroOpinionDeedLog* deedLog;
    if (!deedThing->PeekTC(&deedLog, 0x51) || deedLog == 0) {
        DeliveredReaction = true;
        Terminate();
        return;
    }
    const CCharString* animation = &deedLog->DeedReactions[CurrentDeedType].Animations[attitude];
    unsigned long dialogueTag;
    if (CurrentDeedType != 0x36 && CurrentDeedType != 0x4d) {
        const CTCWife* wife;
        bool rejected = deedLog->PeekPDeedEffectsDef(CurrentDeedType)->IsFlirting && GetCreature().GetSex() == 2 &&
                        GetCreature().GetCreatureType() == 2 && GetCreature().PeekTC(&wife, 0xd7) &&
                        !wife->IsMarriedToThePlayer() && !wife->IsCourtableAndNotBlockingIt();
        if (rejected)
            dialogueTag = deedLog->DeedReactions[0x49].DialogueTags[attitude];
        else {
            dialogueTag = deedLog->DeedReactions[CurrentDeedType].DialogueTags[attitude];
            if ((animation == 0 || animation->IsEmpty()) && dialogueTag == 0 &&
                attitude >= 13) {
                long fallback = 0;
                switch (attitude) {
                case 13: fallback = 12; break;
                case 14: fallback = 9; break;
                case 15: fallback = 0; break;
                case 16: fallback = 8; break;
                case 17: fallback = 3; break;
                }
                dialogueTag = deedLog->DeedReactions[CurrentDeedType].DialogueTags[fallback];
                animation = &deedLog->DeedReactions[CurrentDeedType].Animations[fallback];
            }
        }
    } else {
        CTCCreatureOpinionOfHero* opinion;
        GetCreature().GetTC(&opinion, 0x4f);
        dialogueTag = opinion->GetGreetDialogueGroupIndex(attitude);
    }
    while (GetCreature().PCurrentAction && GetCreature().PCurrentAction->Priority >= 0x32 &&
           !GetCreature().PCurrentAction->IsInterruptable() && !IsStopping())
        PBrain->Yield();
    if (!IS_THING_ALIVE(ThingReactingTo)) {
        Terminate();
        return;
    }
    if (AIAction_LookAtThing((EAIActionRepeat)1, CIntelligentPointer<const CThing>(ThingReactingTo.Get()), 0x21, 0.0027777778f) != 2) {
        Terminate();
        return;
    }
    if (dialogueTag != 0 && reactions.DeedReactionSpeechPlaying[CurrentDeedType] <= 0) {
        CTCTalk* talk;
        if (GetCreature().HasTC(0x0f))
            talk = (CTCTalk*)GetCreature().GetTC(0x0f);
        bool force = deedLog->PeekPDeedEffectsDef(CurrentDeedType)->AllowSpeechOnNonPureAISpeaker;
        CTCTalk::ESpeechPlaybackResult result;
        for (;;) {
            result = talk->PlayAISpeech(dialogueTag, force, deedLog->PeekPDeedEffectsDef(CurrentDeedType)->ReactionDialogueLayer, false, true);
            if (result != 2)
                break;
            if (AIAction_DoNothing() != 4) {
                Terminate();
                return;
            }
        }
        if (result == 0) {
            DeliveredSpeech = true;
            reactions.LogDeedReactionPlayingSpeech(CurrentDeedType, true);
        }
        DeliveredReaction = true;
    }
    if (*animation != "") {
        bool failed = IS_THING_DEAD(ThingReactingTo) ||
                      AIAction_PerformActionTargetFaceAndWait(CCreatureAction_PlayAnimation(&GetCreature(), *animation, CSoundPair(), false,
                                                                                            true, false, 0, 0),
                                                              ThingReactingTo, false) != 2;
        if (failed) {
            Terminate();
            return;
        }
        DeliveredReaction = true;
    }
    if (dialogueTag == 0 && *animation == "")
        DeliveredReaction = true;
    CTCTalk* talk;
    if (GetCreature().HasTC(0x0f))
        talk = (CTCTalk*)GetCreature().GetTC(0x0f);
    while (talk->IsTalking() && !IsStopping())
        PBrain->Yield();
    Terminate();
}
