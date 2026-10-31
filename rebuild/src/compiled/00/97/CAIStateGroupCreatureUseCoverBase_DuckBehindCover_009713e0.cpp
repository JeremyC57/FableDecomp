// CAIStateGroup_CreatureUseCoverBase::DuckBehindCover(bool param) -- with a live target: say the combat dialogue def's "Hiding" speech, then run a projectile-crouch action against the target and report whether it finished (EAIActionReturn 2)
// Leaf: ?DuckBehindCover@CAIStateGroup_CreatureUseCoverBase@@IAE_N_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009713E0, 190 bytes (192 with padding), `ret 4`, frame 0x124 = the action temporary.
// Name: Ego_r.pdb ?DuckBehindCover@CAIStateGroup_CreatureUseCoverBase@@IAE_N_N@Z (protected, bool -> bool; the parked
//   attempts' `long` public spelling was wrong). RTTI (vtable 0x0128D31C): CAIStateGroup_CreatureUseCoverBase :
//   CAIStateGroup_EnemyCreatureBase (PTargetThing +0x14, CIntelligentPointer<CThingCreatureBase>) : CAIStateGroupBase : CBaseClass.
// Callees (all PDB spellings, objdump -r checked): CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3),
//   CAIStateGroupBase::GetWorld 0x008FCFC0 (IBE, CWorld&; x2), CombatDialogueManager::SaySpeech 0x008AFC40,
//   CCreatureAction_ProjectileCrouch::CCreatureAction_ProjectileCrouch(CThingCreatureBase&, CThingCreatureBase&, bool) 0x008B8260,
//   CAIStateGroupBase::AIAction_PerformActionAndWait(const CCreatureActionBase&, bool) 0x0097DB70,
//   CCombatActionBase::~CCombatActionBase 0x0062E900 (landed; virtual, UAE).
// Inline PDB accessors (Ego_r.pdb names, no retail out-of-line copy): CWorld::GetCombatManager (CCombatManager&, the
//   scoped_ptr at +0x4c), CCombatManager::GetCombatDialogueManager (+0x38; Ego_r +0x40, two STLport maps 4 bytes smaller),
//   CombatDialogueManager::GetPDialogueDef (CDefPointer<const CCombatDialogueDef> BY VALUE: the add-ref with a null test and the
//   release whose null test VC7.1 folds away), CAIBrain::GetCreature (CThingAICreature&, +0x18), CThing::IsDead (+0x91 bit 0),
//   CIntelligentPointer<T>::Get / operator-> / operator*.
// Layout: CCombatDialogueDef::Hiding +0x50 (Ego_r +0x54: retail CCombatDialogueDef members are PDB -4, landed IsSpeechAllowed
//   0x008AF5F0). CombatDialogueManager: SpeechGroupInfoMap +0x00 (STLport map 0xC), PDialogueDef +0x0C.
// Shape (the winning levers over the parked 18-line differ): the PDB accessor chain with the shared engine/CDefPointer.h by-value
//   copy (replacing an invented GetDef stand-in) fixed the register rotation; the EAIActionReturn result held in a named local
//   (compared after the temporary's destructor, `mov esi,eax ... cmp esi,2`) and `== 2 ? true : false` (sete al, no xor).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool IsDead() const { return Dead; }            // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CThing* GetPItem() const;                       // 0x00A01B50
    CBaseObject* m_pItem;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
    T* Get() const { return (T*)GetPItem(); }
};

template <class T> __forceinline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CCombatDialogueDef : public CDefClassBase {
public:
    unsigned char _pad[0x50 - sizeof(CDefClassBase)];
    long Hiding;                                    // +0x50 (Ego_r +0x54)
};

class CombatDialogueManager {
public:
    unsigned char _map[0xc];                        // +0x00 SpeechGroupInfoMap (STLport map)
    CDefPointer<const CCombatDialogueDef> PDialogueDef;   // +0x0c
    CDefPointer<const CCombatDialogueDef> GetPDialogueDef() { return PDialogueDef; }   // Ego_r.pdb (inline)
    bool SaySpeech(CThingCreatureBase& creature, unsigned long speech);   // 0x008AFC40
};

class CCombatManager {
public:
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;          // +0x38 (Ego_r +0x40)
    CombatDialogueManager& GetCombatDialogueManager() { return DialogueManager; }   // Ego_r.pdb (inline)
};

#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld : public CWorldLayout {
public:
    CCombatManager& GetCombatManager() { return *PThingCombatManager_ptr; }   // Ego_r.pdb (inline)
};

#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"                       // Creature +0x18 (CThingAICreature*)
#undef CAIBrain
class CAIBrain : public CAIBrainLayout {
public:
    CThingAICreature& GetCreature() const { return *Creature; }   // Ego_r.pdb ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ (inline)
};

// Layout: local model. engine/CCreatureActionBase.h (0xa8, matches retail) keeps the vptr as a data member, so it cannot
//   carry the virtual destructor chain whose ~CCombatActionBase (UAE) retail calls; header debt (NOTES.md).
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa4];
};
class __declspec(novtable) CCombatActionBase : public CCreatureActionBase {
public:
    virtual ~CCombatActionBase();                   // 0x0062E900
    unsigned char _combat[0x6c];
};
class CCreatureAction_ProjectileCrouch : public CCombatActionBase {
public:
    CCreatureAction_ProjectileCrouch(CThingCreatureBase& creature, CThingCreatureBase& target, bool param);   // 0x008B8260
    unsigned char _own[0x10];
};

enum EAIActionReturn { AI_ACTION_DONE = 2 };                 // Ego_r.pdb type; enumerator spelling descriptive (landed rows)

#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"              // vptr +0x00, PBrain +0x04 .. Enabled +0x12 (0x14 bytes)
#undef CAIStateGroupBase
class CAIStateGroupBase : public CAIStateGroupBaseLayout {
protected:
    CWorld& GetWorld() const;                       // 0x008FCFC0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x14
};

class CAIStateGroup_CreatureUseCoverBase : public CAIStateGroup_EnemyCreatureBase {
protected:
    bool DuckBehindCover(bool param);
};

bool CAIStateGroup_CreatureUseCoverBase::DuckBehindCover(bool param)
{
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    unsigned long speech = GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->Hiding;
    GetWorld().GetCombatManager().GetCombatDialogueManager().SaySpeech(PBrain->GetCreature(), speech);
    EAIActionReturn result =
        AIAction_PerformActionAndWait(CCreatureAction_ProjectileCrouch(PBrain->GetCreature(), *PTargetThing, param), false);
    return result == AI_ACTION_DONE ? true : false;
}