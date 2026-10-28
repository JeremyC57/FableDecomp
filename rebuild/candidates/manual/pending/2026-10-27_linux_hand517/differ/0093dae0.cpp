// CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending() const -- true while the creature performs any action or the combat manager has it in a non-interruptable sequence
// Grade: DIFFER(48v48) -- register choice only: the pushed creature loads through edx, retail ecx (GetCreature/local/||/if/bool-local forms probed)
// Leaf: ?IsCombatSequenceActionPending@CAIStateGroup_MeleeCombatNearby2@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DAE0, 48 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): Ego_r.pdb donor order GetCombatManager (inline here), IsCombatSequenceActionPending, IsStillRunnable --
//   retail 0x0093DB10 (the next function) is CAIStateGroup_MeleeCombatNearby2's vtable slot 14 (IsStillRunnable); the body
//   uses exactly the inline ?GetCombatManager@CAIStateGroup_MeleeCombatNearby2@@ABEAAVCCombatManager@@XZ (GetWorld()+0x4C).
// Callees: CThingCreatureBase::IsPerformingAnyAction() const 0x006622A0, CAIStateGroupBase::GetWorld() const 0x008FCFC0,
//   CCombatManager::IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase&) const 0x006E97A0 (landed).
// Shape: `if (creature->IsPerformingAnyAction()) return true; return GetCombatManager().IsCreature...(*creature);`.
// Header debt: engine/CAIStateGroupBase.h / CAIStateGroup_MeleeCombatNearby2.h are flat layouts without bases or methods.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObject : public CBaseClass { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    T* operator->() const { return Data; }
};
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x1C];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    const CWorld& PeekWorld() const;                    // 0x008FCFA0
    CWorld& GetWorld() const;                           // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
class CCombatManager {
public:
    bool IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase& creature) const;   // 0x006E97A0
};
class CAIStateGroup_MeleeCombatNearbyBase : public CAIStateGroupBase { };
class CAIStateGroup_MeleeCombatNearby2 : public CAIStateGroup_MeleeCombatNearbyBase {
private:
    CCombatManager& GetCombatManager() const { return *GetWorld().PThingCombatManager_ptr; }
    bool IsCombatSequenceActionPending() const;
};

bool CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending() const
{
    if (PBrain->Creature->IsPerformingAnyAction())
        return true;
    return GetCombatManager().IsCreaturePerformingNonInterruptableSequence(*PBrain->Creature);
}
