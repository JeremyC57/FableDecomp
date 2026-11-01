// CAIStateGroup_EnemyCreatureBase::OnActivate() -- with a living target: a combat state group registers the creature as the target's combatant, then the leaf's OnActivateLeaf runs; with no living target only the IsCombatStateGroup query is made
// Leaf: ?OnActivate@CAIStateGroup_EnemyCreatureBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091D8C0, length 101 bytes, both exits are virtual tail jumps.
// Identity (proven): slot 2 (OnActivate, after Initialise; CAIStateGroupBase's own slot 2 is the empty 0x008FD26F) of the class's
//   own vtable 0x0128B8BC (RTTI .?AVCAIStateGroup_EnemyCreatureBase@@) and of the 11 derived vtables that inherit it
//   (HoverMovementBase 0x0128A1DC, HoverWander, HoverAttack, HoverAttackUnlessTargetIsFleeing, HoverKeepInArea, HoverAttackArea,
//   HoverHornetQueenTakeDamage, WhisperRetreat, HoverWanderAroundThing, HoverHornetQueen, WhisperPotionCombat); Ego_r.pdb /
//   FableWin.pdb ?OnActivate@CAIStateGroup_EnemyCreatureBase@@UAEXXZ. (The catalog's vtable note "CAIStateGroup_HoverMovementBase#2"
//   is the first inheriting vtable, not the owner.)
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (PTargetThing +0x14, three times), CAIStateGroupBase::GetWorld()
//   const @ 0x008FCFC0, CCombatManager::RegisterCombatant(CThingCreatureBase&, CThingCreatureBase&, bool) @ 0x006EDC50 (FableWin.pdb);
//   own virtual slots 20 (+0x50, IsCombatStateGroup() const: pure here, the leaves' bodies return true for the attack groups and
//   false for the wander / whisper groups -- landed 0x00900C4D etc.) and 16 (+0x40, OnActivateLeaf, the empty 0x008FE0F2).
// Layout: Ego_r.pdb (no STL members before these, so the offsets hold): CAIStateGroupBase PBrain +0x04 (CAIBrain::Creature +0x18
//   through the inline GetCreature()), CAIStateGroup_EnemyCreatureBase PTargetThing +0x14; CWorld PThingCombatManager
//   (boost::scoped_ptr<CCombatManager>) +0x4C through the inline CWorld::GetCombatManager() (FableWin.pdb QAEAAV); CThing dead
//   bit +0x91 bit 0. Vtable order: CAIStateGroupBase slots 1-13 as the retail table (12 = Update, _purecall), then the
//   EnemyCreatureBase additions 14 Init, 15 IsRunnableLeaf, 16 OnActivateLeaf, 17 CleanupLeaf, 18 GetCombatProximity,
//   19 GetCombatMinDistance, 20 IsCombatStateGroup (_purecall), 21 IsRequireDirectAwarenessOfThing, matched against the stub
//   bodies of 0x0128B8BC (ret / mov al,1 / fld). engine/CAIStateGroup_EnemyCreatureBase.h flattens the base: local model.
//   CWorld and CAIBrain come from their generated engine headers (flattened Ego_r layouts, offsets right for retail); the inline
//   members CWorld::GetCombatManager() / CAIBrain::GetCreature() cannot live in those headers, so they are a free inline over
//   PThingCombatManager_ptr and an inline CAIStateGroupBase::GetCreature() over PBrain->Creature (same expansion, no symbol).
// Shape: IS_THING_ALIVE over the intelligent pointer (two GetPItem calls); the creature reference is loaded before GetWorld();
//   OnActivateLeaf is the tail of the living path. Retail's dead-target path tail-calls IsCombatStateGroup() and drops the
//   result: written as that statement (the bytes prove the call; why the source made it is not visible -- a release-mode
//   check that still evaluates its argument would explain it). Matched on the first probe.
#include "engine/CBaseClass.h"

class CBaseObject;
class CBaseObjectPointer;
class CThing { public:                                           // partial view, retail offsets
    virtual ~CThing();
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                                      // +0x91 bit 0
    bool IsDead() const { return Dead; }
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };

class CBaseIntelligentPointer {                                  // vptr + PData (8 bytes)
public:
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                               // 0x00A01B50
    CBaseObjectPointer* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& thing)
{
    return thing.Get() && !thing->IsDead();
}

class CCombatManager { public:
    void RegisterCombatant(CThingCreatureBase& attacker, CThingCreatureBase& target, bool flag);   // 0x006EDC50
};
#include "engine/CWorld.h"                             // Ego_r layout; PThingCombatManager (boost::scoped_ptr<CCombatManager>) +0x4C
#include "engine/CAIBrain.h"                           // Ego_r layout; Creature +0x18
inline CCombatManager& GetCombatManager(CWorld& world) { return *world.PThingCombatManager_ptr; }   // inline CWorld::GetCombatManager()
class CCharString { public: void* PStringData; };
enum EScriptAIPriority { };
enum EAIStateGroupType { };

class CAIStateGroupBase : public CBaseClass {                    // retail vtable 0x01288C60
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 1
    virtual void OnActivate();                                   // 2
    virtual bool InitCreaturePosition();                         // 3
    virtual bool AllowPriorityOverlap();                         // 4
    virtual bool IsStillValid() const;                           // 5
    virtual const CCharString* GetDebugText() const;             // 6
    virtual bool IsTimingCritical() const;                       // 7
    virtual EAIStateGroupType GetStateGroupType() const;         // 8
protected:
    virtual void Cleanup();                                      // 9
    virtual void OnReplace();                                    // 10
    virtual bool IsRunnable();                                   // 11
    virtual void Update() = 0;                                   // 12
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);   // 13
    CWorld& GetWorld() const;                                    // 0x008FCFC0
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }   // inline (PDB IBE), CAIBrain::GetCreature()
    CAIBrain*         PBrain;                                    // +0x04
    EScriptAIPriority ScriptAIPriority;                          // +0x08
    CCharString       Name;                                      // +0x0C
    bool              Terminated;                                // +0x10
    unsigned char     Priority;                                  // +0x11
    bool              Enabled;                                   // +0x12
};

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {   // retail vtable 0x0128B8BC
public:
    virtual void OnActivate();                                   // 2 (0x0091D8C0)
protected:
    virtual void Init();                                         // 14
    virtual bool IsRunnableLeaf();                               // 15
    virtual void OnActivateLeaf();                               // 16 (+0x40)
    virtual void CleanupLeaf();                                  // 17
    virtual float GetCombatProximity() const;                    // 18
    virtual float GetCombatMinDistance() const;                  // 19
public:
    virtual bool IsCombatStateGroup() const = 0;                 // 20 (+0x50)
protected:
    virtual bool IsRequireDirectAwarenessOfThing() const;        // 21
    CIntelligentPointer<CThingCreatureBase> PTargetThing;        // +0x14
};

void CAIStateGroup_EnemyCreatureBase::OnActivate()
{
    if (IS_THING_ALIVE(PTargetThing)) {
        if (IsCombatStateGroup())
            GetCombatManager(GetWorld()).RegisterCombatant(GetCreature(), *PTargetThing, true);
        OnActivateLeaf();
    } else
        IsCombatStateGroup();
}