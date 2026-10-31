// CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending() -- true while the creature is performing any action, else whether the combat manager says it is in a non-interruptable combat sequence
// Leaf: ?IsCombatSequenceActionPending@CAIStateGroup_MeleeCombatNearby2@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DAE0, length 48 bytes (+ int3 padding), private const.
// Callees: CThingCreatureBase::IsPerformingAnyAction() const @ 0x006622A0, CAIStateGroupBase::GetWorld() const
// @ 0x008FCFC0 (protected, returns CWorld&), CCombatManager::IsCreaturePerformingNonInterruptableSequence(const
// CThingCreatureBase&) const @ 0x006E97A0 (catalog + Ego_r.pdb name set; it returns the negated vcall of the
// creature's current combat sequence, `ret 4`).
// Shape: both creature reads use the PDB inline accessors (CAIStateGroupBase::GetCreature() -> CAIBrain::GetCreature(),
// Ego_d.pdb, both returning CThingAICreature&); a raw `*PBrain->Creature` loads the argument through edx. The second test goes through the PDB inline accessor CAIStateGroup_MeleeCombatNearby2::GetCombatManager()
// (Ego_r.pdb `?GetCombatManager@CAIStateGroup_MeleeCombatNearby2@@ABEAAVCCombatManager@@XZ`, = GetWorld().
// GetCombatManager(), itself the PDB inline CWorld::GetCombatManager() over the scoped_ptr at +0x4c) bound to a NAMED reference local. With the reference named, the
// creature argument is read after the world call through ecx (`mov ecx,[esi+4] ; mov ecx,[ecx+18h] ;
// mov eax,[eax+4ch] ; push ecx ; mov ecx,eax`); the unnamed chain `GetCombatManager().f(creature)` or
// `GetWorld().GetCombatManager().f(...)` keeps the creature in a callee-saved register (50 bytes), the earlier
// named world/creature/manager pointers loaded it through edx (48v48). Both inline levels count: dereferencing
// the scoped_ptr straight from the state-group accessor gives `add eax,4ch ; mov eax,[eax]` (50 bytes).
// Layout: the generated CThingCreatureBase / CAIStateGroupBase / CAIBrain / CWorld / CAIStateGroup_MeleeCombatNearby2
// headers flatten the vptr into a `void* __vftable` member (packed layout views), so the classes are modelled locally
// with the header member names (PBrain +0x04, CAIBrain::Creature +0x18, CWorld::PThingCombatManager +0x4c, PEnemy
// +0x14); the headers are included under layout aliases so their offsets stay checkable next to the local model.
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
class CThingCreatureBase { public:
    bool IsPerformingAnyAction() const;                       // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };

class CCombatManager { public:
    bool IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase& creature) const;   // 0x006E97A0
};

namespace boost {
template <class T> class scoped_ptr {
    T* ptr;
public:
    T& operator*() const { return *ptr; }
};
}

class CWorld { public:
    unsigned char _pad_0x00[0x4c];
    boost::scoped_ptr<CCombatManager> PThingCombatManager;    // +0x4c
    CCombatManager& GetCombatManager() { return *PThingCombatManager; }   // Ego_r.pdb ?GetCombatManager@CWorld@@QAEAAVCCombatManager@@XZ
};

class CAIBrain { public:
    unsigned char _pad_0x00[0x18];                            // CSuspendableProcessMicrothread base, Component
    CThingAICreature* Creature;                               // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }           // Ego_d.pdb ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ
};

class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
protected:
    CAIBrain* PBrain;                                         // +0x04
    unsigned char _pad_0x08[0xc];                             // ScriptAIPriority, Name, flags
    CWorld& GetWorld() const;                                 // 0x008FCFC0
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }   // Ego_d.pdb ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

class CAIStateGroup_MeleeCombatNearbyBase : public CAIStateGroupBase {
protected:
    CIntelligentPointer<CThingCreatureBase> PEnemy;           // +0x14
};

class CAIStateGroup_MeleeCombatNearby2 : public CAIStateGroup_MeleeCombatNearbyBase {
    bool Archer;                                              // +0x1c
    CCombatManager& GetCombatManager() const { return GetWorld().GetCombatManager(); }
    bool IsCombatSequenceActionPending() const;
};

bool CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending() const
{
    if (GetCreature().IsPerformingAnyAction())
        return true;
    CCombatManager& manager = GetCombatManager();
    return manager.IsCreaturePerformingNonInterruptableSequence(GetCreature());
}