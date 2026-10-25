// NTCScriptedControl::CActionTalkBase::OnActivate() -- start a talk: mark the action activated, note whether the talker is a demon door and whether the talker or its living target is getting up (knocked down, or in an action that lets animation movement drive the speak-and-wait cameras), drop the talker's conversation mode, remember its conversation attitude, set up the text and speech, re-add a conversation mode for a non-hero talker that can use conversation animations, and reset the combat state of a living hero target
// Grade: DIFFER(522v522) -- register allocation only: the CModeConversation arguments (retail ecx = attitude, edx = parent; ours edx = attitude, then ecx = PTCScript) and the final target block (retail vtable loads edx/eax/edx and `lea eax` for the lookup key; ours eax/edx/eax and `lea ecx`); RTM and QFE alike
// Leaf: ?OnActivate@CActionTalkBase@NTCScriptedControl@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00717710, 522 bytes (+ int3 padding), plain `ret`. Slot 2 of the CActionTalkBase
//   vtable 0x012659A8 (and of the vtable at 0x012659FC); Ego_r.pdb ?OnActivate@CActionTalkBase@NTCScriptedControl@@UAEXXZ.
// Callees: CScriptThing virtual slot 11 GetThing (+0x2C) on PTargetThing; NCreatureMode::IsKnockedDown(const CThing&)
//   @ 0x0085F310 and IsAbleToUseConversationAnimations(const CThing&) @ 0x0085F180 (both `__fastcall`, landed);
//   CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0; the current action's virtual slot 38 (+0x98)
//   IsAllowingAnimationMovementToMoveSpeakAndWaitCameras (the only creature action overriding it is
//   CCreatureAction_PlayAnimation, vtable 0x01273A0C slot 38 = 0x0084A840, its Ego_r.pdb override of that name);
//   CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookups: 0x31 CTCCreatureModeManager = bit 17 of the
//   bitset dword +0x24, 0x30 CTCCombat = bit 16 of +0x24) and its const twin @ 0x004365B0 (0x0F CTCTalk = bit 15 of
//   +0x20, looked up through a const thing); CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 /
//   AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0; CTCTalk::GetAttitude @ 0x006E3180;
//   CActionTalkBase::SetUpTextAndSpeech @ 0x00716B30; operator new @ 0x00BFEA1A;
//   NCreatureMode::CModeConversation(CThing&, EConversationAttitude) @ 0x008D92C0 (0x34 bytes);
//   CCountedPointer<CModeBase>(CModeBase*) @ 0x00718A00 (built in the outgoing argument slot); the CTCCombat method
//   @ 0x007338A0 (no arguments; the landed 0x00855920 names it on CTCCombat, the manifest label is propagated).
// Layout: retail CActionTalkBase = Ego_r.pdb less 4 after the Messages CArray (PreviousConversationAttitude +0x68,
//   AddedConversationMode +0x6C, ParentIsDemonDoor +0x8D, GettingUp +0x8E); retail CThing (local model, as the landed
//   0x0085F180): thing type tag +0x10 (2 = the hero), bitset +0x20, map +0x44, ThingTypeFlags +0x6C (bit 3 =
//   creature), Dead +0x91 bit 0; CThingCreatureBase PCurrentAction +0x100 and the flag byte +0x15D (Ego_r.pdb +0x185:
//   UseMovementInActions, UpdateMovementVectorThisFrame, AbortingCurrentAction, DemonDoor = bit 3).
// Shape: the parent's getting-up test re-reads the parent for the current action (an inline creature accessor written
//   twice); the target's tests call GetThing() again for each use except the creature local; the target's living test
//   is an inline helper over one GetThing() result; the mode manager pointer starts null and is tested again before the
//   mode is re-added.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    void*         __vftable;
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingType;                            // +0x10 (2 = the hero; descriptive)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad50[0x6c - 0x50];
    unsigned long ThingTypeFlags;                       // +0x6C (bit 3 = creature)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                             // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetConstInterface(const T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CCreatureActionBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037();
    virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const;   // slot 38 (+0x98)
};
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* object);                         // 0x00718A00 (CCountedPointer<CModeBase>)
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
    T* operator->() const { return Data; }
};
class CThingCreatureBase : public CThing {
public:
    unsigned char _pad94[0x100 - 0x94];
    CCountedPointer<CCreatureActionBase> PCurrentAction;   // +0x100
    unsigned char _pad108[0x15d - 0x108];
    bool UseMovementInActions : 1;                      // +0x15D bit 0
    bool UpdateMovementVectorThisFrame : 1;
    bool AbortingCurrentAction : 1;
    bool DemonDoor : 1;                                 // bit 3
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};

enum EConversationAttitude { CONVERSATION_ATTITUDE_NONE = 0 };
class CTCTalk {
public:
    EConversationAttitude GetAttitude() const;          // 0x006E3180
};
class CTCCombat {
public:
    void ResetCombatState();                            // 0x007338A0 (descriptive)
};
class CTCCreatureModeManager {
public:
    class CModeBase { };
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
    bool AddMode(CCountedPointer<CModeBase> mode);      // 0x0073ACC0
};

namespace NCreatureMode {
bool __fastcall IsKnockedDown(const CThing& thing);                       // 0x0085F310
bool __fastcall IsAbleToUseConversationAnimations(const CThing& thing);   // 0x0085F180
class CModeConversation : public CTCCreatureModeManager::CModeBase {      // 0x34 bytes
public:
    CModeConversation(CThing& thing, EConversationAttitude attitude);     // 0x008D92C0
    unsigned char _pad[0x34];
};
}

struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    CScriptThing*   PImp_Data;                          // +0x04
    CCPPointerInfo* PImp_Info;                          // +0x08
};

struct CTCScriptedControl {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
};

__forceinline bool IsLivingThing(const CThing* thing) { return thing && !thing->Dead; }

namespace NTCScriptedControl {

enum { CREATURE_MODE_CONVERSATION = 27, THING_TYPE_HERO = 2, THING_TYPE_FLAG_CREATURE = 8 };   // descriptive

class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                              // slot 1
    virtual void OnActivate();                          // slot 2
    CTCScriptedControl* PTCScript;                      // +0x04
    bool                Activated;                      // +0x08
    bool                Terminated;                     // +0x09
};

class CActionTalkBase : public CActionBase {
public:
    virtual void OnActivate();
    CThingCreatureBase* GetParentCreature() const { return (CThingCreatureBase*)PTCScript->ParentThing; }
    void SetUpTextAndSpeech();                          // 0x00716B30
    CScriptThing          PTargetThing;                 // +0x0C
    unsigned char         _pad18[0x68 - 0x18];          // UpdateCounter .. CameraToBeMovedDueToMovedSetupPositionsCount
    EConversationAttitude PreviousConversationAttitude; // +0x68
    bool                  AddedConversationMode;        // +0x6C
    unsigned char         _pad6d[0x8d - 0x6d];          // FarEnoughAwayForFacingCamera .. ThingLookingPosSet
    bool                  ParentIsDemonDoor;            // +0x8D
    bool                  GettingUp;                    // +0x8E
};

void CActionTalkBase::OnActivate()
{
    Activated = true;
    AddedConversationMode = false;
    ParentIsDemonDoor = GetParentCreature()->DemonDoor;
    if (NCreatureMode::IsKnockedDown(*PTCScript->ParentThing) ||
        (GetParentCreature()->IsPerformingAnyAction() &&
         GetParentCreature()->PCurrentAction->IsAllowingAnimationMovementToMoveSpeakAndWaitCameras()))
        GettingUp = true;
    if (IsLivingThing(PTargetThing.GetThing())) {
        CThingCreatureBase* creature;
        if (NCreatureMode::IsKnockedDown(*PTargetThing.GetThing()) ||
            ((PTargetThing.GetThing()->ThingTypeFlags & THING_TYPE_FLAG_CREATURE) &&
             (creature = (CThingCreatureBase*)PTargetThing.GetThing())->IsPerformingAnyAction() &&
             creature->PCurrentAction->IsAllowingAnimationMovementToMoveSpeakAndWaitCameras()))
            GettingUp = true;
    }
    CTCCreatureModeManager* modes = 0;
    if (PTCScript->ParentThing->GetInterface(&modes, 0x31))
        modes->RemoveMode(CREATURE_MODE_CONVERSATION);
    const CTCTalk* talk;
    if (PTCScript->ParentThing->GetConstInterface(&talk, 0x0f))
        PreviousConversationAttitude = talk->GetAttitude();
    SetUpTextAndSpeech();
    if (PTCScript->ParentThing->ThingType != THING_TYPE_HERO && modes &&
        NCreatureMode::IsAbleToUseConversationAnimations(*PTCScript->ParentThing)) {
        modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(
            new NCreatureMode::CModeConversation(*PTCScript->ParentThing, PreviousConversationAttitude)));
        AddedConversationMode = true;
    }
    CTCCombat* combat;
    if (IsLivingThing(PTargetThing.GetThing()) && PTargetThing.GetThing()->ThingType == THING_TYPE_HERO &&
        PTargetThing.GetThing()->GetInterface(&combat, 0x30))
        combat->ResetCombatState();
}
}
