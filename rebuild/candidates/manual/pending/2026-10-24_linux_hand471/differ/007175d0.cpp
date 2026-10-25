// NTCScriptedControl::CActionSummonerLightningAttackTarget::Update() -- turn the summoner to face the target first; then (once) hand the creature a lightning-orb-into attack on the target, finishing at once if the summoner is already in the orb mode (90) or the target is dead; after the attack started, finish once the creature is idle and its current mode is no longer the orb mode
// Grade: DIFFER(318v318) -- register allocation only: retail gives the attack-started block's creature ebx (the register that held the SetCurrentAction bool), so ebx is pushed at the head with ebp / edi; VC7.1 here reuses ebp for that creature and shrink-wraps the ebx push into the attack-start block (frame words then shift by 4)
// Leaf: ?Update@CActionSummonerLightningAttackTarget@NTCScriptedControl@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007175D0, 318 bytes (+ int3 padding), plain `ret`. Slot 1 of vtable 0x0126586C
//   (rtti-vtables.tsv; the embedded turn action's slot 1 is CActionTurnToFacePosition::Update 0x00714CC0).
// Callees: the turn action's virtual Update (slot 1, through the member: VC7.1 does not devirtualise member calls), the
//   CScriptThing slots 75 IsAlive (+0x12C) and 11 GetPThing (+0x2C) on PTarget, CVectorMap::LowerBound @ 0x004365B0
//   (the inlined interface lookup of type 0x31 = bit 17 of the bitset dword +0x24), CTCCreatureModeManager::HasMode
//   @ 0x0073A8A0 / GetCurrentMode @ 0x0073A6E0 (manifest "CPlayer::HasMode" / "CPixelShader::CountReferences" are
//   propagated; landed names of 0x008CB4A0 / 0x00830F20) and the mode's virtual GetType (slot 1),
//   CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase*, CThing*) @ 0x008CBD90, CThingCreatureBase::
//   SetCurrentAction @ 0x006644F0 / IsPerformingAnyAction @ 0x006622A0, ~CBaseIntelligentPointer @ 0x00A01C10 and the
//   combat-action base destructor @ 0x0062E900 (manifest "~CTCLightningOrb" propagated).
// Layout: the landed constructor 0x00713180 (CActionBase PTCScript +4 / Terminated +9, PTarget +0x0C, StartedAttack
//   +0x18, TurnedToFace +0x19, TurnToFaceAction +0x1C with its Terminated at +0x25); the orb action is 0x11C bytes in
//   retail (CCombatActionBase 0x114 as the landed 0x008CB4A0, then the target intelligent pointer +0x114).
// Shape: the orb action is a temporary bound to SetCurrentAction's const reference (the constructor's returned `this`
//   is pushed) whose implicit destructor inlines at the end of the full expression (member pointer, then the base
//   destructor) with the SetCurrentAction result held in bl; the mode-manager lookups are the out-pointer GetInterface template
//   (the second one never checks the result: its absent path reads the never-written frame word shared with the key).
class CThing;
class CTCBase;
struct CInterfaceMap {                                  // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x004365B0
};

class CMode {
public:
    virtual void v00();
    virtual long GetType() const;                       // slot 1
};

class CTCCreatureModeManager {
public:
    bool   HasMode(unsigned long mode);                 // 0x0073A8A0
    CMode* GetCurrentMode();                            // 0x0073A6E0
};

class CCreatureActionBase;
class CThingCreatureBase {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    bool IsPerformingAnyAction();                       // 0x006622A0
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0 (Ego_r.pdb: by const reference)
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
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
};
class CCombatActionBase : public CCreatureActionBase {  // retail sizeof 0x114
public:
    virtual ~CCombatActionBase();                       // 0x0062E900
    unsigned char _pad04[0x114 - 4];
};
class CCreatureAction_SummonerLightningOrbInto : public CCombatActionBase {   // retail sizeof 0x11C
public:
    CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase* creature, CThing* target);   // 0x008CBD90
    CIntelligentPointer<CThing> PTarget;                // +0x114
};

class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetPThing() const;                  // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp_Data;                                    // +0x04
    void* PImp_Info;                                    // +0x08
};

struct CTCScriptedControl {
    void*               __vftable;
    CThingCreatureBase* ParentThing;                    // +0x04
};

namespace NTCScriptedControl {

enum { CREATURE_MODE_SUMMONER_LIGHTNING_ORB = 90 };     // descriptive
enum { TC_CREATURE_MODE_MANAGER = 0x31 };

class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                              // slot 1
    CTCScriptedControl* PTCScript;                      // +0x04
    bool                Activated;                      // +0x08
    bool                Terminated;                     // +0x09
};

class CActionTurnToFacePosition : public CActionBase {  // vtable 0x01265534, 0x1c bytes
public:
    virtual void Update();                              // 0x00714CC0
    float Position_X, Position_Y, Position_Z;           // +0x0c
    float Tolerance;                                    // +0x18
};

class CActionSummonerLightningAttackTarget : public CActionBase {
public:
    virtual void Update();
    CScriptThing              PTarget;                  // +0x0c
    bool                      StartedAttack;            // +0x18
    bool                      TurnedToFace;             // +0x19
    CActionTurnToFacePosition TurnToFaceAction;         // +0x1c
};

void CActionSummonerLightningAttackTarget::Update()
{
    if (!TurnedToFace) {
        TurnToFaceAction.Update();
        if (!TurnToFaceAction.Terminated)
            return;
        TurnedToFace = true;
    }
    if (!StartedAttack) {
        CThingCreatureBase* creature = PTCScript->ParentThing;
        CTCCreatureModeManager* modes;
        if (creature->GetInterface(&modes, TC_CREATURE_MODE_MANAGER) && modes->HasMode(CREATURE_MODE_SUMMONER_LIGHTNING_ORB)) {
            Terminated = true;
            return;
        }
        if (!PTarget.IsAlive()) {
            Terminated = true;
            return;
        }
        if (creature->SetCurrentAction(CCreatureAction_SummonerLightningOrbInto(creature, PTarget.GetPThing())))
            StartedAttack = true;
    }
    if (StartedAttack) {
        CThingCreatureBase* creature = PTCScript->ParentThing;
        CTCCreatureModeManager* modes;
        creature->GetInterface(&modes, TC_CREATURE_MODE_MANAGER);
        if (!creature->IsPerformingAnyAction() && modes->GetCurrentMode()->GetType() != CREATURE_MODE_SUMMONER_LIGHTNING_ORB)
            Terminated = true;
    }
}
}
