// CCombatManager::GetNextActionForCreature(CThingCreatureBase&) -- reuse eligible sequence action or reset and advance the sequence
// Retail0x006EB600,247 bytes. PDB map16 -> retail tree12 shifts Combatants20->16; native node pair data+24.
// Creature PCurrentAction is native+0x100, donor+0x11c; UID inherited at+0x18. Local retail view documents that mismatch.
// Leaf: ?GetNextActionForCreature@CCombatManager@@QAEPAVCCombatActionBase@@AAVCThingCreatureBase@@@Z
// Name fix (NF199, 2026-11-20, q4389): the invented CombatMap::FindLE / CombatNode / CreatureUID stand-ins are replaced by the STLport
//   map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h) keyed by the creature's 64-bit UniqueID (+0x18); find's out-of-line
//   _M_find<unsigned __int64> 0x006F0250 is declared only (FABLE_RETAIL_STLP_TREE_MFIND_DECL), so the call binds the self row's spelling.
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
class CCombatActionBase;
class CThingCreatureBase { public:
    unsigned char _beforeUID[0x18];unsigned __int64 UID;unsigned char _beforeCurrentAction[0xe0];
    CCreatureActionBase* PCurrentAction_Data;
    bool IsPerformingAnyAction() const; //0x006622A0
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};
class CCombatSequenceBase { public:
    virtual ~CCombatSequenceBase();
    virtual void OnCreate();
    virtual CCharString GetName() const;
    virtual long GetCombatSequenceType() const;
    virtual long GetCombatSequenceInterruptionType() const;
    virtual bool IsInterruptable() const;
    virtual bool IsInterruptableDueToZoneChange() const; //slot6 verified0x006F06F0
    bool Running;
    bool IsZoneValid() const; //0x008B0610
    CCombatActionBase* GetNextActionInSequence(); //0x008B0780
};
struct SequencePointer { CCombatSequenceBase* Data; void* Info; };
class CAttackingCombatantInfo { public:
    CThingCreatureBase* Attacked;long AttackerNumber,CombatWheelRingIndex;
    bool PreferredAttackDollySet;long PreferredAttackDollyTimer;float PreferredAttackDollyVector[2];bool UsingCombatWheel;
    long CurrentSequence;SequencePointer* Begin;SequencePointer* End;SequencePointer* Capacity;
};
struct CCombatantInfo { CThingCreatureBase* Parent;CAttackingCombatantInfo AttackerInfo; };
class CCombatManager { public:
    void* VTable;unsigned char RegisteredCombatSequences[12];std::map<unsigned __int64, CCombatantInfo> Combatants;
    private: void PerformSequenceReset(CThingCreatureBase&); public: //0x006EA2C0 (NF308: AAE, both PDBs)
    private:
    void SetupNextSequenceForAttacker(CCombatantInfo&);
    public:
 //0x006EB390
    CCombatActionBase* GetNextActionForCreature(CThingCreatureBase& creature);
};
CCombatActionBase* CCombatManager::GetNextActionForCreature(CThingCreatureBase& creature){
    if(!creature.IsInMeleeAsAttacker())return 0;
    unsigned __int64 key=creature.UID;
    CCombatantInfo& info=Combatants.find(key)->second;
    bool canAct=true;
    if(creature.IsPerformingAnyAction() && !creature.PCurrentAction_Data->Finished)canAct=false;
    if(info.AttackerInfo.CurrentSequence>=0){
        CCombatSequenceBase* sequence=info.AttackerInfo.Begin[info.AttackerInfo.CurrentSequence].Data;
        if(sequence->Running && (sequence->IsZoneValid() || !sequence->IsInterruptableDueToZoneChange())){
            CCombatActionBase* action=sequence->GetNextActionInSequence();
            if(action)return canAct?action:0;
        }
        if(!canAct)return 0;
        PerformSequenceReset(creature);
    }
    SetupNextSequenceForAttacker(info);
    if(creature.IsInMeleeAsAttacker() && info.AttackerInfo.CurrentSequence>=0)
        return info.AttackerInfo.Begin[info.AttackerInfo.CurrentSequence].Data->GetNextActionInSequence();
    return 0;
}